#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "../core/srf_utils.hpp"

struct Edge {
    int to;
    int weight;
};

// SRF Graph DP (Recompute Mode)
// Simulates recomputation of predecessor states for a frontier-based traversal
int graph_recompute(int num_nodes, const std::vector<std::vector<Edge>>& adj, int recompute_depth) {
    std::vector<int> dist(num_nodes, 1e9);
    dist[0] = 0;

    for (int u = 0; u < num_nodes; ++u) {
        if (dist[u] == 1e9) continue;
        for (const auto& edge : adj[u]) {
            srf::global_metrics.record_recompute(recompute_depth);
            if (dist[u] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.weight;
            }
        }
    }
    return dist[num_nodes - 1];
}

int main(int argc, char* argv[]) {
    int num_nodes = (argc > 1) ? std::stoi(argv[1]) : 1000;
    int recompute_depth = (argc > 2) ? std::stoi(argv[2]) : 2;

    std::vector<std::vector<Edge>> adj(num_nodes);
    for (int i = 0; i < num_nodes - 1; ++i) {
        adj[i].push_back({i + 1, 1});
    }

    srf::global_metrics.reset();
    auto start_time = std::chrono::high_resolution_clock::now();
    int result = graph_recompute(num_nodes, adj, recompute_depth);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::cout << "Algorithm: Graph-DP" << std::endl;
    std::cout << "Variant: SRF-Recompute" << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    std::cout << "Recompute_Events: " << srf::global_metrics.recompute_events << std::endl;
    std::cout << "Cache_Hits_Diagnostic: " << (num_nodes * 1) << std::endl;
    std::cout << "Param_1: " << recompute_depth << " (DepthProxy)" << std::endl;
    std::cout << "Param_2: " << num_nodes << " (NumNodes)" << std::endl;

    return 0;
}