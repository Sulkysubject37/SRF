#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "../utils.hpp"

struct Scoring {
    int match = 1;
    int mismatch = -1;
    int gap = -1;
};

std::string load_file(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return "";
    std::string s;
    f >> s;
    return s;
}

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
    // Usage: ./needleman_wunsch [PathA] [PathB]
    if (argc < 3) return 1;
    std::string s1 = load_file(argv[1]);
    std::string s2 = load_file(argv[2]);

    Scoring score;
    int result = run_nw(s1, s2, score);

    std::cout << "Algorithm: Needleman-Wunsch" << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: 0" << std::endl; // Baseline timing not critical for equivalence test
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    
    return 0;
}
