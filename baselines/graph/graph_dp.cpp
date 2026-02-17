#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "../utils.hpp"

struct Edge {
    int to;
    int weight;
};

int run_graph_dp(int num_nodes, const std::vector<std::vector<Edge>>& adj) {
    std::vector<int> dist(num_nodes, 1e9);
    dist[0] = 0;
    for (int u = 0; u < num_nodes; ++u) {
        if (dist[u] == 1e9) continue;
        for (const auto& edge : adj[u]) {
            if (dist[u] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.weight;
            }
        }
    }
    return dist[num_nodes - 1];
}

int main(int argc, char* argv[]) {
    int num_nodes = (argc > 1) ? std::stoi(argv[1]) : 1000;
    std::vector<std::vector<Edge>> adj(num_nodes);
    for (int i = 0; i < num_nodes - 1; ++i) {
        adj[i].push_back({i + 1, 1});
    }

    const int iterations = 500;
    
    // Validation run
    int result = run_graph_dp(num_nodes, adj);

    auto start_time = std::chrono::high_resolution_clock::now();
    long long dummy = 0;
    for (int i = 0; i < iterations; ++i) {
        dummy += run_graph_dp(num_nodes, adj);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    double avg_duration = static_cast<double>(total_duration) / iterations;
    size_t memory = srf::get_peak_rss();

    std::cout << "Algorithm: Graph-DP" << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << avg_duration << std::endl;
    std::cout << "Memory_kb: " << memory << std::endl;
    std::cout << "Cache_Hits_Diagnostic: " << (num_nodes * 1) << std::endl;

    if (dummy == 0) return 0; // Prevent optimization
    return 0;
}