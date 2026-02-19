#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include "../core/srf_utils.hpp"
#include "../runtime/backend_selector.h"
#include "../granularity/granularity_policy.h"

enum Observation { Walk, Shop, Clean, Unknown };

Observation char_to_obs(char c) {
    if (c == 'A') return Walk;
    if (c == 'C') return Shop;
    if (c == 'G') return Clean;
    return Unknown;
}

std::vector<Observation> load_observations(const std::string& path) {
    std::ifstream f(path);
    std::string s;
    f >> s;
    std::vector<Observation> obs;
    for(char c : s) obs.push_back(char_to_obs(c));
    return obs;
}

double viterbi_granularity_aware(const std::vector<Observation>& obs, int K, int G, srf::IBackend* backend) {
    double start_p[] = {0.6, 0.4};
    double trans_p[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
    double emit_p[2][4] = {{0.1, 0.4, 0.4, 0.1}, {0.6, 0.2, 0.1, 0.1}};
    size_t T = obs.size();
    size_t S = 2;

    srf::GranularityPolicy policy(srf::GranularityType::SEGMENT, G);
    std::vector<std::vector<double>> checkpoints((T / K) + 1, std::vector<double>(S));
    std::vector<double> V(S);
    srf::global_metrics.update_working_set((checkpoints.size() * S + S) * sizeof(double));

    for (size_t s = 0; s < S; ++s) {
        V[s] = start_p[s] * emit_p[s][obs[0]];
        srf::global_metrics.record_compute(1);
        srf::global_metrics.record_mem_access();
    }
    checkpoints[0] = V;

    for (size_t t = 1; t < T; ++t) {
        bool is_checkpoint = (t % K == 0);
        if (!is_checkpoint) {
            srf::global_metrics.record_recompute(1);
            srf::global_metrics.record_unit_recompute(policy.get_unit_id(t));
        }
        
        std::vector<double> next_V(S);
        for (size_t s = 0; s < S; ++s) {
            std::vector<double> trans_row = {trans_p[0][s], trans_p[1][s]};
            next_V[s] = backend->viterbi_step_compute(V, trans_row, emit_p[s][obs[t]]);
            srf::global_metrics.record_compute(1);
            srf::global_metrics.record_mem_access();
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
    // Usage: ./viterbi_checkpoint [Path] [K] [G] [ScaleLabel]
    if (argc < 4) return 1;
    std::vector<Observation> obs = load_observations(argv[1]);
    int K = std::stoi(argv[2]);
    int G = std::stoi(argv[3]);
    std::string scale = (argc > 4) ? argv[4] : "NA";

    auto backend = srf::BackendSelector::select(1024);
    srf::global_metrics.reset();
    backend->reset_metrics();
    
    auto start_time = std::chrono::high_resolution_clock::now();
    double result = viterbi_granularity_aware(obs, K, G, backend.get());
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    auto b_metrics = backend->get_metrics();

    std::cout << "Algorithm: Viterbi" << std::endl;
    std::cout << "Dataset_Scale: " << scale << std::endl;
    std::cout << "Input_Size: " << obs.size() << std::endl;
    std::cout << "Backend: " << (backend->type() == srf::BackendType::GPU ? "gpu" : "cpu") << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    std::cout << "Recompute_Events: " << srf::global_metrics.recompute_events << std::endl;
    std::cout << "Compute_Events: " << srf::global_metrics.compute_events << std::endl;
    std::cout << "Memory_Access_Proxy: " << srf::global_metrics.memory_access_proxy << std::endl;
    std::cout << "Working_Set_Proxy: " << srf::global_metrics.working_set_bytes << std::endl;
    std::cout << "Granularity_Unit_Size: " << G << std::endl;
    std::cout << "Unit_Recompute_Events: " << srf::global_metrics.unit_recompute_events << std::endl;
    std::cout << "Unit_Reuse_Proxy: " << srf::global_metrics.unit_reuse_proxy << std::endl;
    std::cout << "Dispatch_Overhead_Proxy: " << srf::global_metrics.dispatch_overhead_proxy << std::endl;
    std::cout << "Param_1: " << K << std::endl;
    std::cout << "Param_3: " << G << std::endl;

    return 0;
}
