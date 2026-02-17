#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

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

int main() {
    // Larger sequences for measurable performance
    std::string s1(200, 'A');
    std::string s2(200, 'T');
    for(int i=0; i<200; i+=5) s1[i] = 'T'; // Add some variety
    
    Scoring score;
    const int iterations = 100;
    
    auto start = std::chrono::high_resolution_clock::now();
    int dummy = 0;
    for (int i = 0; i < iterations; ++i) {
        dummy += run_nw(s1, s2, score);
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double avg_duration = static_cast<double>(total_duration) / iterations;

    std::cout << "Algorithm: Needleman-Wunsch" << std::endl;
    std::cout << "Final_Score_Check: " << dummy << std::endl; // Ensure computation isn't optimized away
    std::cout << "Time_us: " << avg_duration << std::endl;
    return 0;
}