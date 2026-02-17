#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <cmath>

// Simple HMM: 2 states (Rainy, Sunny), 3 observations (Walk, Shop, Clean)
enum State { Rainy, Sunny };
enum Observation { Walk, Shop, Clean };

void viterbi() {
    std::vector<Observation> obs = {Walk, Shop, Clean};
    std::vector<State> states = {Rainy, Sunny};

    // Probabilities
    double start_p[] = {0.6, 0.4};
    double trans_p[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
    double emit_p[2][3] = {{0.1, 0.4, 0.5}, {0.6, 0.3, 0.1}};

    size_t T = obs.size();
    size_t S = states.size();

    std::vector<std::vector<double>> V(T, std::vector<double>(S));
    std::vector<std::vector<int>> path(T, std::vector<int>(S));

    auto start_time = std::chrono::high_resolution_clock::now();

    // Initial step
    for (size_t s = 0; s < S; ++s) {
        V[0][s] = start_p[s] * emit_p[s][obs[0]];
        path[0][s] = 0;
    }

    // Recursion
    for (size_t t = 1; t < T; ++t) {
        for (size_t s = 0; s < S; ++s) {
            double max_p = -1.0;
            int best_prev_s = 0;
            for (size_t prev_s = 0; prev_s < S; ++prev_s) {
                double p = V[t - 1][prev_s] * trans_p[prev_s][s] * emit_p[s][obs[t]];
                if (p > max_p) {
                    max_p = p;
                    best_prev_s = prev_s;
                }
            }
            V[t][s] = max_p;
            path[t][s] = best_prev_s;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    // Traceback
    double final_max_p = -1.0;
    int last_state = 0;
    for (size_t s = 0; s < S; ++s) {
        if (V[T - 1][s] > final_max_p) {
            final_max_p = V[T - 1][s];
            last_state = s;
        }
    }

    std::cout << "Algorithm: Viterbi" << std::endl;
    std::cout << "Observations: " << T << std::endl;
    std::cout << "Max_Probability: " << final_max_p << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
}

int main() {
    viterbi();
    return 0;
}
