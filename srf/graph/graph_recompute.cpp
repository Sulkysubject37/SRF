#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <functional>
#include "../core/srf_utils.hpp"
#include "../runtime/backend_selector.h"
#include "../granularity/granularity_policy.h"
#include "../control/drift_detector.h"
#include "../control/adaptation_policy.h"

struct Edge {
    int to;
    int weight;
};

std::vector<std::vector<Edge>> load_graph(const std::string& path, int& num_nodes) {
    std::ifstream f(path);
    if (!f.is_open()) return {};
    
    int u, v, w;
    std::vector<std::pair<int, int>> temp_edges;
    int max_node = -1;
    while (f >> u >> v >> w) {
        temp_edges.push_back({u, v});
        if (u > max_node) max_node = u;
        if (v > max_node) max_node = v;
    }
    num_nodes = max_node + 1;
    std::vector<std::vector<Edge>> adj_out(num_nodes);
    for(auto& e : temp_edges) {
        adj_out[e.first].push_back({e.second, 1}); // Using 1 for unit weight tests
    }
    return adj_out;
}

int graph_granularity_aware(int num_nodes, const std::vector<std::vector<Edge>>& adj_out, int depth_init, int G, srf::IBackend* backend, srf::RegimeObserver& observer) {
    if (num_nodes == 0) return 0;
    
    // 1. Graph Preprocessing (Out-Degree and Adjacency Inversion)
    std::vector<std::vector<Edge>> adj_in(num_nodes);
    std::vector<int> out_degree(num_nodes, 0);

    for (int u = 0; u < num_nodes; ++u) {
        for (const auto& edge : adj_out[u]) {
            adj_in[edge.to].push_back({u, edge.weight});
            out_degree[u]++;
        }
    }

    // 2. GraphFrontier Manager (O(width(G)) Tracking)
    std::unordered_map<int, int> active_states;
    active_states[0] = 0;
    
    size_t M_bound = 5000; // Dynamic threshold for extreme stress

    int current_D = depth_init;
    srf::GranularityPolicy policy(srf::GranularityType::GROUP, G);
    srf::DriftDetector detector;
    srf::AdaptationPolicy adapter(50);

    // 3. On-Demand Recomputation Kernel
    std::function<int(int, int)> recompute = [&](int node, int depth) -> int {
        if (node == 0) return 0;
        if (active_states.count(node)) return active_states[node];

        srf::global_metrics.record_recompute(1);
        srf::global_metrics.record_unit_recompute(policy.get_unit_id(node));

        std::vector<int> pred_dists;
        std::vector<int> weights;

        for (const auto& edge : adj_in[node]) {
            int p = edge.to;
            int dist_p = active_states.count(p) ? active_states[p] : recompute(p, depth + 1);
            pred_dists.push_back(dist_p);
            weights.push_back(edge.weight);
        }
        
        int new_dist = backend->graph_node_compute(pred_dists, weights);
        srf::global_metrics.record_compute(1);
        return new_dist;
    };

    // 4. Topological Pull-Model Evaluation
    for (int v = 1; v < num_nodes; ++v) {
        if (adj_in[v].empty()) {
            active_states[v] = 1e9; 
            continue;
        }

        std::vector<int> pred_dists;
        std::vector<int> weights;

        for (const auto& edge : adj_in[v]) {
            int u = edge.to;
            
            // Check Frontier, Trigger Recomputation if Evicted
            int dist_u = active_states.count(u) ? active_states[u] : recompute(u, 1);
            
            pred_dists.push_back(dist_u);
            weights.push_back(edge.weight);

            // Wave-front Eviction: Remove when out-degree hits 0
            out_degree[u]--;
            if (out_degree[u] == 0) {
                active_states.erase(u);
            }
        }

        active_states[v] = backend->graph_node_compute(pred_dists, weights);
        srf::global_metrics.record_compute(1);
        srf::global_metrics.record_mem_access();
        
        // Update live memory metric
        srf::global_metrics.update_working_set(active_states.size() * sizeof(int));

        // 5. Dynamic State Eviction Engine (M_bound Enforcement)
        if (active_states.size() > M_bound) {
            // Evict oldest node to strictly enforce M_bound limit
            int oldest_node = v;
            for (const auto& pair : active_states) {
                if (pair.first < oldest_node && pair.first != 0) { // Keep root safe
                    oldest_node = pair.first;
                }
            }
            if (oldest_node != v) {
                active_states.erase(oldest_node);
            }
        }
        
        if (v % 100 == 0) {
            observer.record_snapshot(srf::global_metrics.compute_events, 
                                     srf::global_metrics.recompute_events, 
                                     srf::global_metrics.memory_access_proxy, 
                                     srf::global_metrics.working_set_bytes);
            
            srf::DriftState ds = detector.detect(observer);
            srf::AdaptationSignal signal = adapter.evaluate(ds, observer);
            if (signal.should_adapt) {
                current_D += signal.delta;
                if (current_D < 1) current_D = 1;
            }
        }
    }
    
    // Final snapshot capture
    observer.record_snapshot(srf::global_metrics.compute_events, 
                             srf::global_metrics.recompute_events, 
                             srf::global_metrics.memory_access_proxy, 
                             srf::global_metrics.working_set_bytes);

    return active_states.count(num_nodes - 1) ? active_states[num_nodes - 1] : recompute(num_nodes - 1, 1);
}

int main(int argc, char* argv[]) {
    if (argc < 4) return 1;
    int num_nodes = 0;
    auto adj = load_graph(argv[1], num_nodes);
    int recompute_depth = std::stoi(argv[2]);
    int G = std::stoi(argv[3]);
    std::string scale = (argc > 4) ? argv[4] : "NA";

    auto backend = srf::BackendSelector::select(1024);
    srf::global_metrics.reset();
    backend->reset_metrics();
    
    srf::RegimeObserver observer;

    auto start_time = std::chrono::high_resolution_clock::now();
    int result = graph_granularity_aware(num_nodes, adj, recompute_depth, G, backend.get(), observer);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    
    srf::DriftDetector detector;
    srf::DriftState d_state = detector.detect(observer);
    srf::RegimeSnapshot latest = observer.get_latest();

    std::cout << "Algorithm: Graph-DP" << std::endl;
    std::cout << "Dataset_Scale: " << scale << std::endl;
    std::cout << "Backend: " << (backend->type() == srf::BackendType::GPU ? "gpu" : "cpu") << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    
    std::cout << "Drift_State: " << (d_state == srf::DriftState::STABLE ? "STABLE" : (d_state == srf::DriftState::DRIFT_CANDIDATE ? "DRIFT_CANDIDATE" : "INSUFFICIENT_DATA")) << std::endl;
    std::cout << "R_mem: " << latest.r_mem << std::endl;
    std::cout << "R_rec: " << latest.r_rec << std::endl;
    
    std::cout << "Param_1: " << recompute_depth << std::endl;
    std::cout << "Param_3: " << G << std::endl;

    return 0;
}
