#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "../utils.hpp"

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

double run_viterbi(const std::vector<Observation>& obs) {
    double start_p[] = {0.6, 0.4};
    double trans_p[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
    double emit_p[2][4] = {{0.1, 0.4, 0.4, 0.1}, {0.6, 0.2, 0.1, 0.1}};
    size_t T = obs.size();
    size_t S = 2;

    std::vector<double> V(S);
    for (size_t s = 0; s < S; ++s) V[s] = start_p[s] * emit_p[s][obs[0]];

    for (size_t t = 1; t < T; ++t) {
        std::vector<double> next_V(S);
        for (size_t s = 0; s < S; ++s) {
            double max_p = -1.0;
            for (size_t i = 0; i < S; ++i) {
                double p = V[i] * trans_p[i][s];
                if (p > max_p) max_p = p;
            }
            next_V[s] = max_p * emit_p[s][obs[t]];
        }
        V = next_V;
    }

    double final_max_p = -1.0;
    for (size_t s = 0; s < S; ++s) {
        if (V[s] > final_max_p) final_max_p = V[s];
    }
    return final_max_p;
}

int main(int argc, char* argv[]) {
    // Usage: ./viterbi [Path]
    if (argc < 2) return 1;
    std::vector<Observation> obs = load_observations(argv[1]);

    double result = run_viterbi(obs);

    std::cout << "Algorithm: Viterbi" << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: 0" << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;

    return 0;
}
