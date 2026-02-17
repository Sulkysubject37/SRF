#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <cmath>

enum State { Rainy, Sunny };
enum Observation { Walk, Shop, Clean };

double run_forward(const std::vector<Observation>& obs, const std::vector<State>& states) {
    double start_p[] = {0.6, 0.4};
    double trans_p[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
    double emit_p[2][3] = {{0.1, 0.4, 0.5}, {0.6, 0.3, 0.1}};

    size_t T = obs.size();
    size_t S = states.size();

    std::vector<std::vector<double>> alpha(T, std::vector<double>(S));

    for (size_t s = 0; s < S; ++s) {
        alpha[0][s] = start_p[s] * emit_p[s][obs[0]];
    }

    for (size_t t = 1; t < T; ++t) {
        for (size_t s = 0; s < S; ++s) {
            double sum_p = 0.0;
            for (size_t prev_s = 0; prev_s < S; ++prev_s) {
                sum_p += alpha[t - 1][prev_s] * trans_p[prev_s][s] * emit_p[s][obs[t]];
            }
            alpha[t][s] = sum_p;
        }
    }

    double total_prob = 0.0;
    for (size_t s = 0; s < S; ++s) {
        total_prob += alpha[T - 1][s];
    }
    return total_prob;
}

int main() {
    std::vector<Observation> obs(100, Walk);
    for(int i=0; i<100; i+=3) obs[i] = Shop;
    std::vector<State> states = {Rainy, Sunny};

    const int iterations = 1000;

    auto start_time = std::chrono::high_resolution_clock::now();
    double dummy = 0;
    for (int i = 0; i < iterations; ++i) {
        dummy += run_forward(obs, states);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    double avg_duration = static_cast<double>(total_duration) / iterations;

    std::cout << "Algorithm: Forward" << std::endl;
    std::cout << "Forward_Check: " << dummy << std::endl;
    std::cout << "Time_us: " << avg_duration << std::endl;
    return 0;
}