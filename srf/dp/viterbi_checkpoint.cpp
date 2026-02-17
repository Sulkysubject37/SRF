#include <iostream>
#include <vector>
#include <chrono>
#include "../core/srf_utils.hpp"

enum State { Rainy, Sunny };
enum Observation { Walk, Shop, Clean };

double viterbi_locality_aware(const std::vector<Observation>& obs, int K_base, int locality_mode) {
    double start_p[] = {0.6, 0.4};
    double trans_p[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
    double emit_p[2][3] = {{0.1, 0.4, 0.5}, {0.6, 0.3, 0.1}};
    size_t T = obs.size();
    size_t S = 2;

    int K = K_base;
    if (locality_mode > 0) {
        K = (K_base > 4) ? K_base / 2 : 2;
    }

    std::vector<std::vector<double>> checkpoints((T / K) + 1, std::vector<double>(S));
    std::vector<double> V(S);
    srf::global_metrics.update_working_set((checkpoints.size() * S + S) * sizeof(double));

    for (size_t s = 0; s < S; ++s) V[s] = start_p[s] * emit_p[s][obs[0]];
    checkpoints[0] = V;

    for (size_t t = 1; t < T; ++t) {
        bool is_checkpoint = (t % K == 0);
        if (!is_checkpoint) {
            srf::global_metrics.record_recompute(1);
            srf::global_metrics.record_dist(t % K);
        }
        
        std::vector<double> next_V(S);
        for (size_t s = 0; s < S; ++s) {
            double max_p = -1.0;
            for (size_t prev_s = 0; prev_s < S; ++prev_s) {
                double p = V[prev_s] * trans_p[prev_s][s] * emit_p[s][obs[t]];
                if (p > max_p) max_p = p;
            }
            next_V[s] = max_p;
        }
        V = next_V;
        if (is_checkpoint) checkpoints[t / K] = V;
    }

    double final_max_p = -1.0;
    for (size_t s = 0; s < S; ++s) {
        if (V[s] > final_max_p) final_max_p = V[s];
    }
    return final_max_p;
}

int main(int argc, char* argv[]) {
    int seq_len = (argc > 1) ? std::stoi(argv[1]) : 500;
    int k_interval = (argc > 2) ? std::stoi(argv[2]) : 10;
    int loc_mode = (argc > 3) ? std::stoi(argv[3]) : 0;

    std::vector<Observation> obs(seq_len, Walk);
    for(int i=0; i<seq_len; i+=3) obs[i] = Shop;

    srf::global_metrics.reset();
    auto start_time = std::chrono::high_resolution_clock::now();
    double result = viterbi_locality_aware(obs, k_interval, loc_mode);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::cout << "Algorithm: Viterbi" << std::endl;
    std::cout << "Variant: SRF-LocalityAware" << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    std::cout << "Recompute_Events: " << srf::global_metrics.recompute_events << std::endl;
    std::cout << "Locality_Proxy: " << srf::global_metrics.total_dist_metric << std::endl;
    std::cout << "Working_Set_Proxy: " << srf::global_metrics.working_set_bytes << std::endl;
    std::cout << "Param_1: " << k_interval << " (K-Interval)" << std::endl;
    std::cout << "Param_2: " << seq_len << " (SeqLen)" << std::endl;
    std::cout << "Param_3: " << loc_mode << " (LocalityMode)" << std::endl;

    return 0;
}