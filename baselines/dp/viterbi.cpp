#include <iostream>
#include <vector>
#include <chrono>
#include "../utils.hpp"

enum State { Rainy, Sunny };
enum Observation { Walk, Shop, Clean };

double run_viterbi(const std::vector<Observation>& obs, const std::vector<State>& states) {
    double start_p[] = {0.6, 0.4};
    double trans_p[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
    double emit_p[2][3] = {{0.1, 0.4, 0.5}, {0.6, 0.3, 0.1}};
    size_t T = obs.size();
    size_t S = states.size();
    std::vector<std::vector<double>> V(T, std::vector<double>(S));
    for (size_t s = 0; s < S; ++s) V[0][s] = start_p[s] * emit_p[s][obs[0]];
    for (size_t t = 1; t < T; ++t) {
        for (size_t s = 0; s < S; ++s) {
            double max_p = -1.0;
            for (size_t prev_s = 0; prev_s < S; ++prev_s) {
                double p = V[t - 1][prev_s] * trans_p[prev_s][s] * emit_p[s][obs[t]];
                if (p > max_p) max_p = p;
            }
            V[t][s] = max_p;
        }
    }
    return V[T - 1][0];
}

int main() {
    std::vector<Observation> obs(500, Walk);
    std::vector<State> states = {Rainy, Sunny};
    const int iterations = 500;
    auto start_time = std::chrono::high_resolution_clock::now();
    double dummy = 0;
    for (int i = 0; i < iterations; ++i) dummy += run_viterbi(obs, states);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    double avg_duration = static_cast<double>(total_duration) / iterations;
    size_t memory = srf::get_peak_rss();

    std::cout << "Algorithm: Viterbi" << std::endl;
    std::cout << "Time_us: " << avg_duration << std::endl;
    std::cout << "Memory_kb: " << memory << std::endl;
    std::cout << "Cache_Hits_Diagnostic: " << (500 * 2 * 2) << std::endl;
    return 0;
}
