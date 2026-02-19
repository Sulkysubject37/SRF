#include <iostream>
#include <vector>
#include <chrono>
#include "../core/srf_utils.hpp"
#include "../runtime/backend_selector.h"
#include "../granularity/granularity_policy.h"

enum Observation { Walk, Shop, Clean };

double forward_granularity_aware(const std::vector<Observation>& obs, int K, int G, srf::IBackend* backend) {
    double start_p[] = {0.6, 0.4};
    double trans_p[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
    double emit_p[2][3] = {{0.1, 0.4, 0.5}, {0.6, 0.3, 0.1}};
    size_t T = obs.size();
    size_t S = 2;

    srf::GranularityPolicy policy(srf::GranularityType::SEGMENT, G);

    std::vector<std::vector<double>> checkpoints((T / K) + 1, std::vector<double>(S));
    std::vector<double> alpha(S);
    srf::global_metrics.update_working_set((checkpoints.size() * S + S) * sizeof(double));

    for (size_t s = 0; s < S; ++s) alpha[s] = start_p[s] * emit_p[s][obs[0]];
    checkpoints[0] = alpha;

    for (size_t t = 1; t < T; ++t) {
        bool is_checkpoint = (t % K == 0);
        if (!is_checkpoint) {
            srf::global_metrics.record_recompute(1);
            srf::global_metrics.record_unit_recompute(policy.get_unit_id(t));
        }
        
        std::vector<double> next_alpha(S);
        for (size_t s = 0; s < S; ++s) {
            std::vector<double> trans_row = {trans_p[0][s], trans_p[1][s]};
            next_alpha[s] = backend->forward_step_compute(alpha, trans_row, emit_p[s][obs[t]]);
        }
        alpha = next_alpha;
        if (is_checkpoint) checkpoints[t / K] = alpha;
    }

    double total_prob = 0.0;
    for (size_t s = 0; s < S; ++s) total_prob += alpha[s];
    return total_prob;
}

int main(int argc, char* argv[]) {
    int seq_len = (argc > 1) ? std::stoi(argv[1]) : 500;
    int K = (argc > 2) ? std::stoi(argv[2]) : 10;
    int G = (argc > 3) ? std::stoi(argv[3]) : 1;
    size_t gpu_budget = (argc > 4) ? std::stoul(argv[4]) : 1024;

    auto backend = srf::BackendSelector::select(gpu_budget);

    std::vector<Observation> obs(seq_len, Walk);
    for(int i=0; i<seq_len; i+=3) obs[i] = Shop;

    srf::global_metrics.reset();
    backend->reset_metrics();
    auto start_time = std::chrono::high_resolution_clock::now();
    double result = forward_granularity_aware(obs, K, G, backend.get());
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    auto b_metrics = backend->get_metrics();

    std::cout << "Algorithm: Forward" << std::endl;
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
    std::cout << "Param_1: " << K << std::endl;
    std::cout << "Param_2: " << seq_len << std::endl;
    std::cout << "Param_3: " << G << std::endl;

    return 0;
}
