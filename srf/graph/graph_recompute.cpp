#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "../core/srf_utils.hpp"
#include "../runtime/backend_selector.h"

struct Edge {
    int to;
    int weight;
};

int graph_locality_aware(int num_nodes, const std::vector<std::vector<Edge>>& adj, int recompute_depth, int schedule_mode, srf::IBackend* backend) {
    std::vector<int> dist(num_nodes, 1e9);
    dist[0] = 0;
    srf::global_metrics.update_working_set(dist.size() * sizeof(int));

    std::vector<int> schedule(num_nodes);
    for (int i = 0; i < num_nodes; ++i) schedule[i] = i;

    if (schedule_mode == 1) {
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
            srf::global_metrics.record_dist(std::abs(edge.to - u));
            
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
    int sched_mode = (argc > 3) ? std::stoi(argv[3]) : 0;
    size_t gpu_budget = (argc > 4) ? std::stoul(argv[4]) : 1024;

    auto backend = srf::BackendSelector::select(gpu_budget);

    std::vector<std::vector<Edge>> adj(num_nodes);
    for (int i = 0; i < num_nodes - 1; ++i) {
        adj[i].push_back({i + 1, 1});
    }

    srf::global_metrics.reset();
    backend->reset_metrics();
    auto start_time = std::chrono::high_resolution_clock::now();
    int result = graph_locality_aware(num_nodes, adj, recompute_depth, sched_mode, backend.get());
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    auto b_metrics = backend->get_metrics();

    std::cout << "Algorithm: Graph-DP" << std::endl;
    std::cout << "Variant: SRF-LocalityAware" << std::endl;
    std::cout << "Backend: " << (backend->type() == srf::BackendType::GPU ? "gpu" : "cpu") << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    std::cout << "Recompute_Events: " << srf::global_metrics.recompute_events << std::endl;
    std::cout << "Transfer_Overhead_us: " << b_metrics.transfer_overhead_us << std::endl;
    std::cout << "Kernel_Launch_Count: " << b_metrics.kernel_launch_count << std::endl;
    std::cout << "Device_Memory_Budget_kb: " << b_metrics.device_memory_budget_kb << std::endl;
    std::cout << "Param_1: " << recompute_depth << std::endl;
    std::cout << "Param_2: " << num_nodes << std::endl;
    std::cout << "Param_3: " << sched_mode << std::endl;

    return 0;
}