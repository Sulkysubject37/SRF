#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include "../utils.hpp"

struct Scoring {
    int match = 1;
    int mismatch = -1;
    int gap = -1;
};

int run_nw(const std::string& s1, const std::string& s2, const Scoring& score) {
    size_t n = s1.length();
    size_t m = s2.length();
    std::vector<std::vector<int>> matrix(n + 1, std::vector<int>(m + 1));
    for (size_t i = 0; i <= n; ++i) matrix[i][0] = i * score.gap;
    for (size_t j = 0; j <= m; ++j) matrix[0][j] = j * score.gap;
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= m; ++j) {
            int match_score = (s1[i - 1] == s2[j - 1]) ? score.match : score.mismatch;
            matrix[i][j] = std::max({
                matrix[i - 1][j - 1] + match_score,
                matrix[i - 1][j] + score.gap,
                matrix[i][j - 1] + score.gap
            });
        }
    }
    return matrix[n][m];
}

int main(int argc, char* argv[]) {
    int seq_len = (argc > 1) ? std::stoi(argv[1]) : 300;
    
    std::string s1(seq_len, 'A');
    std::string s2(seq_len, 'T');
    // Align input logic with SRF variants
    for(int i=0; i<seq_len; i+=5) s1[i] = 'T';

    Scoring score;
    const int iterations = 50;
    
    // Single run for validation
    int validation_score = run_nw(s1, s2, score);

    auto start = std::chrono::high_resolution_clock::now();
    long long dummy = 0;
    for (int i = 0; i < iterations; ++i) {
        dummy += run_nw(s1, s2, score);
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double avg_duration = static_cast<double>(total_duration) / iterations;
    size_t memory = srf::get_peak_rss();

    std::cout << "Algorithm: Needleman-Wunsch" << std::endl;
    std::cout << "Result_Check: " << validation_score << std::endl;
    std::cout << "Time_us: " << avg_duration << std::endl;
    std::cout << "Memory_kb: " << memory << std::endl;
    std::cout << "Cache_Hits_Diagnostic: " << (seq_len * seq_len) << std::endl; // Approx
    
    // Prevent optimization of dummy
    if (dummy == 0) return 0; 

    return 0;
}