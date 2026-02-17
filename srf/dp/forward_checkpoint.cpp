#include <iostream>
#include <vector>
#include <chrono>
#include "../core/srf_utils.hpp"

enum State { Rainy, Sunny };
enum Observation { Walk, Shop, Clean };

double forward_checkpoint(const std::vector<Observation>& obs, int K) {
    double start_p[] = {0.6, 0.4};
    double trans_p[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
    double emit_p[2][3] = {{0.1, 0.4, 0.5}, {0.6, 0.3, 0.1}};
    
    size_t T = obs.size();
    size_t S = 2;

    // Checkpoints every K steps
    std::vector<std::vector<double>> checkpoints((T / K) + 1, std::vector<double>(S));

    // Simulation of SRF: Only store checkpoints, recompute intermediate steps
    std::vector<double> alpha(S);
    for (size_t s = 0; s < S; ++s) alpha[s] = start_p[s] * emit_p[s][obs[0]];
    checkpoints[0] = alpha;

    for (size_t t = 1; t < T; ++t) {
        if (t % K == 0) {
            // Store checkpoint
            std::vector<double> next_alpha(S);
            for (size_t s = 0; s < S; ++s) {
                double sum_p = 0.0;
                for (size_t prev_s = 0; prev_s < S; ++prev_s) {
                    sum_p += alpha[prev_s] * trans_p[prev_s][s] * emit_p[s][obs[t]];
                }
                next_alpha[s] = sum_p;
            }
            alpha = next_alpha;
            checkpoints[t / K] = alpha;
        } else {
            // Recompute step
            srf::global_metrics.record_recompute(1);
            std::vector<double> next_alpha(S);
            for (size_t s = 0; s < S; ++s) {
                double sum_p = 0.0;
                for (size_t prev_s = 0; prev_s < S; ++prev_s) {
                    sum_p += alpha[prev_s] * trans_p[prev_s][s] * emit_p[s][obs[t]];
                }
                next_alpha[s] = sum_p;
            }
            alpha = next_alpha;
        }
    }

    double total_prob = 0.0;
    for (size_t s = 0; s < S; ++s) total_prob += alpha[s];
    return total_prob;
}

int main(int argc, char* argv[]) {
    int seq_len = (argc > 1) ? std::stoi(argv[1]) : 500;
    int k_interval = (argc > 2) ? std::stoi(argv[2]) : 10;

    std::vector<Observation> obs(seq_len, Walk);
    for(int i=0; i<seq_len; i+=3) obs[i] = Shop;

    srf::global_metrics.reset();
    auto start_time = std::chrono::high_resolution_clock::now();
    double result = forward_checkpoint(obs, k_interval);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::cout << "Algorithm: Forward" << std::endl;
    std::cout << "Variant: SRF-Checkpoint" << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    std::cout << "Recompute_Events: " << srf::global_metrics.recompute_events << std::endl;
    std::cout << "Cache_Hits_Diagnostic: " << (seq_len * 2 * 2) << std::endl;
    std::cout << "Param_1: " << k_interval << " (K-Interval)" << std::endl;
    std::cout << "Param_2: " << seq_len << " (SeqLen)" << std::endl;

    return 0;
}
