#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "../core/srf_utils.hpp"

struct Edge {
    int to;
    int weight;
};

// SRF Graph DP (Locality-Aware Recompute)
int graph_locality_aware(int num_nodes, const std::vector<std::vector<Edge>>& adj, int recompute_depth, int schedule_mode) {
    std::vector<int> dist(num_nodes, 1e9);
    dist[0] = 0;
    srf::global_metrics.update_working_set(dist.size() * sizeof(int));

    // Schedule: 0 = Natural (0..N), 1 = Interleaved for cache tests
    std::vector<int> schedule(num_nodes);
    for (int i = 0; i < num_nodes; ++i) schedule[i] = i;

    if (schedule_mode == 1) {
        // Deterministic interleave: 0, 2, 4... then 1, 3, 5...
        std::vector<int> next_sched;
        for (int i = 0; i < num_nodes; i += 2) next_sched.push_back(i);
        for (int i = 1; i < num_nodes; i += 2) next_sched.push_back(i);
        schedule = next_sched;
    }

    for (int idx = 0; idx < num_nodes; ++idx) {
        int u = schedule[idx];
        if (dist[u] == 1e9) continue;
        for (const auto& edge : adj[u]) {
            srf::global_metrics.record_recompute(recompute_depth);
            // Locality proxy: distance in schedule between u and to
            srf::global_metrics.record_dist(std::abs(edge.to - u));
            
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
    int sched_mode = (argc > 3) ? std::stoi(argv[3]) : 0;

    std::vector<std::vector<Edge>> adj(num_nodes);
    for (int i = 0; i < num_nodes - 1; ++i) {
        adj[i].push_back({i + 1, 1});
    }

    srf::global_metrics.reset();
    auto start_time = std::chrono::high_resolution_clock::now();
    int result = graph_locality_aware(num_nodes, adj, recompute_depth, sched_mode);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::cout << "Algorithm: Graph-DP" << std::endl;
    std::cout << "Variant: SRF-LocalityAware" << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    std::cout << "Recompute_Events: " << srf::global_metrics.recompute_events << std::endl;
    std::cout << "Locality_Proxy: " << srf::global_metrics.total_dist_metric << std::endl;
    std::cout << "Working_Set_Proxy: " << srf::global_metrics.working_set_bytes << std::endl;
    std::cout << "Param_1: " << recompute_depth << " (DepthProxy)" << std::endl;
    std::cout << "Param_2: " << num_nodes << " (NumNodes)" << std::endl;
    std::cout << "Param_3: " << sched_mode << " (ScheduleMode)" << std::endl;

    return 0;
}
