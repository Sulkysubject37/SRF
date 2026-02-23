#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "../core/srf_utils.hpp"
#include "../runtime/backend_selector.h"
#include "../granularity/granularity_policy.h"
#include "../control/drift_detector.h"

struct Edge {
    int to;
    int weight;
};

std::vector<std::vector<Edge>> load_graph(const std::string& path, int& num_nodes) {
    std::ifstream f(path);
    int u, v, w;
    std::vector<std::pair<int, int>> temp_edges;
    int max_node = -1;
    while (f >> u >> v >> w) {
        temp_edges.push_back({u, v});
        if (u > max_node) max_node = u;
        if (v > max_node) max_node = v;
    }
    num_nodes = max_node + 1;
    std::vector<std::vector<Edge>> adj(num_nodes);
    for(auto& e : temp_edges) {
        adj[e.first].push_back({e.second, 1});
    }
    return adj;
}

int graph_granularity_aware(int num_nodes, const std::vector<std::vector<Edge>>& adj, int recompute_depth, int G, srf::IBackend* backend, srf::RegimeObserver& observer) {
    if (num_nodes == 0) return 0;
    std::vector<int> dist(num_nodes, 1e9);
    dist[0] = 0;
    srf::global_metrics.record_mem_access();
    srf::global_metrics.update_working_set(dist.size() * sizeof(int));

    srf::GranularityPolicy policy(srf::GranularityType::GROUP, G);

    for (int u = 0; u < num_nodes; ++u) {
        if (dist[u] == 1e9) continue;
        srf::global_metrics.record_mem_access();
        for (const auto& edge : adj[u]) {
            srf::global_metrics.record_recompute(recompute_depth);
            srf::global_metrics.record_unit_recompute(policy.get_unit_id(u));
            
            std::vector<int> pred_dists = {dist[u]};
            std::vector<int> weights = {edge.weight};
            int new_dist = backend->graph_node_compute(pred_dists, weights);
            srf::global_metrics.record_compute(1);
            srf::global_metrics.record_mem_access();
            srf::global_metrics.record_mem_access();
            
            if (new_dist < dist[edge.to]) {
                dist[edge.to] = new_dist;
                srf::global_metrics.record_mem_access();
            }
        }
        
        // Phase 8A: Record snapshot periodically
        if (u % 100 == 0) {
            observer.record_snapshot(srf::global_metrics.compute_events, 
                                     srf::global_metrics.recompute_events, 
                                     srf::global_metrics.memory_access_proxy, 
                                     srf::global_metrics.working_set_bytes);
        }
    }
    return dist[num_nodes - 1];
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
    srf::DriftDetector detector;

    auto start_time = std::chrono::high_resolution_clock::now();
    int result = graph_granularity_aware(num_nodes, adj, recompute_depth, G, backend.get(), observer);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    
    srf::DriftState d_state = detector.detect(observer);
    srf::RegimeSnapshot latest = observer.get_latest();

    std::cout << "Algorithm: Graph-DP" << std::endl;
    std::cout << "Dataset_Scale: " << scale << std::endl;
    std::cout << "Input_Size: " << num_nodes << std::endl;
    std::cout << "Backend: " << (backend->type() == srf::BackendType::GPU ? "gpu" : "cpu") << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    
    // Phase 8A Logging
    std::cout << "Drift_State: " << (d_state == srf::DriftState::STABLE ? "STABLE" : (d_state == srf::DriftState::DRIFT_CANDIDATE ? "DRIFT_CANDIDATE" : "INSUFFICIENT_DATA")) << std::endl;
    std::cout << "R_mem: " << latest.r_mem << std::endl;
    std::cout << "R_rec: " << latest.r_rec << std::endl;

    return 0;
}
