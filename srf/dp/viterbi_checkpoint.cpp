#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "../core/srf_utils.hpp"
#include "../runtime/backend_selector.h"
#include "../granularity/granularity_policy.h"
#include "../control/drift_detector.h"
#include "../control/adaptation_policy.h"

enum Observation { Walk, Shop, Clean, OBS_UNKNOWN };

Observation char_to_obs(char c) {
    if (c == 'A') return Walk;
    if (c == 'C') return Shop;
    if (c == 'G') return Clean;
    return OBS_UNKNOWN;
}

std::vector<Observation> load_observations(const std::string& path) {
    std::ifstream f(path);
    std::string s;
    f >> s;
    std::vector<Observation> obs;
    for(char c : s) obs.push_back(char_to_obs(c));
    return obs;
}

struct Checkpoint {
    size_t t;
    std::vector<double> V;
};

double viterbi_granularity_aware(const std::vector<Observation>& obs, int K_init, int G, srf::IBackend* backend, srf::RegimeObserver& observer) {
    double start_p[] = {0.6, 0.4};
    double trans_p[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
    double emit_p[2][4] = {{0.1, 0.4, 0.4, 0.1}, {0.6, 0.2, 0.1, 0.1}};
    size_t T = obs.size();
    size_t S = 2;
    if (T == 0) return 0.0;

    int current_K = K_init;
    srf::GranularityPolicy policy(srf::GranularityType::SEGMENT, G);
    srf::DriftDetector detector;
    srf::AdaptationPolicy adapter(100);

    std::vector<Checkpoint> checkpoints;
    std::vector<double> V(S);
    srf::global_metrics.update_working_set((T / K_init + 2) * S * sizeof(double));

    for (size_t s = 0; s < S; ++s) {
        V[s] = start_p[s] * emit_p[s][obs[0]];
        srf::global_metrics.record_compute(1);
        srf::global_metrics.record_mem_access();
    }
    checkpoints.push_back({0, V});
    size_t last_checkpoint_t = 0;

    // 1. FORWARD PASS
    for (size_t t = 1; t < T; ++t) {
        std::vector<double> next_V(S);
        for (size_t s = 0; s < S; ++s) {
            std::vector<double> trans_row = {trans_p[0][s], trans_p[1][s]};
            next_V[s] = backend->viterbi_step_compute(V, trans_row, emit_p[s][obs[t]]);
            srf::global_metrics.record_compute(1);
            srf::global_metrics.record_mem_access();
        }
        V = next_V;
        
        // Adaptive Checkpointing
        if (t - last_checkpoint_t >= (size_t)current_K || t == T - 1) {
            checkpoints.push_back({t, V});
            last_checkpoint_t = t;
        }
        
        if (t % 10 == 0) {
            observer.record_snapshot(srf::global_metrics.compute_events, 
                                     srf::global_metrics.recompute_events, 
                                     srf::global_metrics.memory_access_proxy, 
                                     srf::global_metrics.working_set_bytes);
            
            srf::DriftState ds = detector.detect(observer);
            srf::AdaptationSignal signal = adapter.evaluate(ds, observer);
            if (signal.should_adapt) {
                int old_K = current_K;
                current_K += signal.delta;
                if (current_K < 2) current_K = 2;
                // Optional: Output adaptation event
                // std::cout << "ADAPTATION_EVENT: true, NEW_PARAM: " << current_K << std::endl;
            }
        }
    }

    double final_max_p = -1.0;
    int best_last_state = 0;
    for (size_t s = 0; s < S; ++s) {
        if (V[s] > final_max_p) {
            final_max_p = V[s];
            best_last_state = s;
        }
    }

    // 2. BACKWARD PASS (Traceback with On-Demand Recomputation)
    std::vector<int> optimal_path(T);
    optimal_path[T - 1] = best_last_state;

    // Iterate backwards through checkpoints
    for (int c = checkpoints.size() - 2; c >= 0; --c) {
        size_t start_t = checkpoints[c].t;
        size_t end_t = checkpoints[c+1].t;
        if (start_t == end_t) continue;

        size_t segment_len = end_t - start_t;
        std::vector<std::vector<double>> local_V(segment_len + 1, std::vector<double>(S));
        local_V[0] = checkpoints[c].V;

        srf::global_metrics.update_working_set((checkpoints.size() + segment_len) * S * sizeof(double));

        // Recomputation Kernel
        for (size_t i = 1; i <= segment_len; ++i) {
            size_t t = start_t + i;
            
            // ACTUAL Recompute record (replacing the mocked metric)
            srf::global_metrics.record_recompute(1);
            srf::global_metrics.record_unit_recompute(policy.get_unit_id(t));

            for (size_t s = 0; s < S; ++s) {
                std::vector<double> trans_row = {trans_p[0][s], trans_p[1][s]};
                local_V[i][s] = backend->viterbi_step_compute(local_V[i-1], trans_row, emit_p[s][obs[t]]);
                srf::global_metrics.record_mem_access();
            }
        }

        // Local Traceback
        for (size_t i = segment_len; i > 0; --i) {
            size_t t = start_t + i;
            if (t == T - 1) continue; // Already set terminal state

            int next_state = optimal_path[t];
            double max_trans_p = -1.0;
            int best_prev_state = 0;

            for (size_t prev_s = 0; prev_s < S; ++prev_s) {
                double p = local_V[i-1][prev_s] * trans_p[prev_s][next_state] * emit_p[next_state][obs[t]];
                if (p > max_trans_p) {
                    max_trans_p = p;
                    best_prev_state = prev_s;
                }
            }
            optimal_path[t - 1] = best_prev_state;
        }
    }

    observer.record_snapshot(srf::global_metrics.compute_events, srf::global_metrics.recompute_events, srf::global_metrics.memory_access_proxy, srf::global_metrics.working_set_bytes);
    return final_max_p;
}

int main(int argc, char* argv[]) {
    if (argc < 4) return 1;
    std::vector<Observation> obs = load_observations(argv[1]);
    int K = std::stoi(argv[2]);
    int G = std::stoi(argv[3]);
    std::string scale = (argc > 4) ? argv[4] : "NA";

    auto backend = srf::BackendSelector::select(1024);
    srf::global_metrics.reset();
    backend->reset_metrics();
    
    srf::RegimeObserver observer;

    auto start_time = std::chrono::high_resolution_clock::now();
    double result = viterbi_granularity_aware(obs, K, G, backend.get(), observer);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    
    srf::DriftDetector detector;
    srf::DriftState d_state = detector.detect(observer);
    srf::RegimeSnapshot latest = observer.get_latest();

    std::cout << "Algorithm: Viterbi" << std::endl;
    std::cout << "Dataset_Scale: " << scale << std::endl;
    std::cout << "Backend: " << (backend->type() == srf::BackendType::GPU ? "gpu" : "cpu") << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Drift_State: " << (d_state == srf::DriftState::STABLE ? "STABLE" : (d_state == srf::DriftState::DRIFT_CANDIDATE ? "DRIFT_CANDIDATE" : "INSUFFICIENT_DATA")) << std::endl;
    std::cout << "R_mem: " << latest.r_mem << std::endl;
    std::cout << "R_rec: " << latest.r_rec << std::endl;
    std::cout << "Param_1: " << K << std::endl;
    std::cout << "Param_3: " << G << std::endl;

    return 0;
}
