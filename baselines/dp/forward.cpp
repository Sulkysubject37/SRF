#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <cmath>

// Simple HMM: 2 states (Rainy, Sunny), 3 observations (Walk, Shop, Clean)
enum State { Rainy, Sunny };
enum Observation { Walk, Shop, Clean };

void forward_algorithm() {
    std::vector<Observation> obs = {Walk, Shop, Clean};
    std::vector<State> states = {Rainy, Sunny};

    // Probabilities
    double start_p[] = {0.6, 0.4};
    double trans_p[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
    double emit_p[2][3] = {{0.1, 0.4, 0.5}, {0.6, 0.3, 0.1}};

    size_t T = obs.size();
    size_t S = states.size();

    std::vector<std::vector<double>> alpha(T, std::vector<double>(S));

    auto start_time = std::chrono::high_resolution_clock::now();

    // Initial step
    for (size_t s = 0; s < S; ++s) {
        alpha[0][s] = start_p[s] * emit_p[s][obs[0]];
    }

    // Recursion
    for (size_t t = 1; t < T; ++t) {
        for (size_t s = 0; s < S; ++s) {
            double sum_p = 0.0;
            for (size_t prev_s = 0; prev_s < S; ++prev_s) {
                sum_p += alpha[t - 1][prev_s] * trans_p[prev_s][s] * emit_p[s][obs[t]];
            }
            alpha[t][s] = sum_p;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    // Termination
    double total_prob = 0.0;
    for (size_t s = 0; s < S; ++s) {
        total_prob += alpha[T - 1][s];
    }

    std::cout << "Algorithm: Forward" << std::endl;
    std::cout << "Observations: " << T << std::endl;
    std::cout << "Total_Probability: " << total_prob << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
}

int main() {
    forward_algorithm();
    return 0;
}
