#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "../core/srf_utils.hpp"
#include "../runtime/backend_selector.h"
#include "../granularity/granularity_policy.h"

struct Edge {
    int to;
    int weight;
};

int graph_granularity_aware(int num_nodes, const std::vector<std::vector<Edge>>& adj, int recompute_depth, int G, srf::IBackend* backend) {
    std::vector<int> dist(num_nodes, 1e9);
    dist[0] = 0;
    srf::global_metrics.update_working_set(dist.size() * sizeof(int));

    srf::GranularityPolicy policy(srf::GranularityType::GROUP, G);

    for (int u = 0; u < num_nodes; ++u) {
        if (dist[u] == 1e9) continue;
        for (const auto& edge : adj[u]) {
            srf::global_metrics.record_recompute(recompute_depth);
            srf::global_metrics.record_unit_recompute(policy.get_unit_id(u));
            
            // Use backend primitive
            std::vector<int> pred_dists = {dist[u]};
            std::vector<int> weights = {edge.weight};
            int new_dist = backend->graph_node_compute(pred_dists, weights);
            
            if (new_dist < dist[edge.to]) {
                dist[edge.to] = new_dist;
            }
        }
    }
    return dist[num_nodes - 1];
}

int main(int argc, char* argv[]) {
    int num_nodes = (argc > 1) ? std::stoi(argv[1]) : 1000;
    int recompute_depth = (argc > 2) ? std::stoi(argv[2]) : 2;
    int G = (argc > 3) ? std::stoi(argv[3]) : 1;
    size_t gpu_budget = (argc > 4) ? std::stoul(argv[4]) : 1024;

    auto backend = srf::BackendSelector::select(gpu_budget);

    std::vector<std::vector<Edge>> adj(num_nodes);
    for (int i = 0; i < num_nodes - 1; ++i) {
        adj[i].push_back({i + 1, 1});
    }

    srf::global_metrics.reset();
    backend->reset_metrics();
    auto start_time = std::chrono::high_resolution_clock::now();
    int result = graph_granularity_aware(num_nodes, adj, recompute_depth, G, backend.get());
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    auto b_metrics = backend->get_metrics();

    std::cout << "Algorithm: Graph-DP" << std::endl;
    std::cout << "Variant: SRF-GranularityAware" << std::endl;
    std::cout << "Backend: " << (backend->type() == srf::BackendType::GPU ? "gpu" : "cpu") << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    std::cout << "Recompute_Events: " << srf::global_metrics.recompute_events << std::endl;
    std::cout << "Unit_Recompute_Events: " << srf::global_metrics.unit_recompute_events << std::endl;
    std::cout << "Unit_Reuse_Proxy: " << srf::global_metrics.unit_reuse_proxy << std::endl;
    std::cout << "Granularity_Unit_Size: " << G << std::endl;
    std::cout << "Transfer_Overhead_us: " << b_metrics.transfer_overhead_us << std::endl;
    std::cout << "Kernel_Launch_Count: " << b_metrics.kernel_launch_count << std::endl;
    std::cout << "Param_1: " << recompute_depth << std::endl;
    std::cout << "Param_2: " << num_nodes << std::endl;
    std::cout << "Param_3: " << G << std::endl;

    return 0;
}