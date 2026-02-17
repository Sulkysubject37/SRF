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

void needleman_wunsch(const std::string& s1, const std::string& s2) {
    Scoring score;
    size_t n = s1.length();
    size_t m = s2.length();

    std::vector<std::vector<int>> matrix(n + 1, std::vector<int>(m + 1));

    for (size_t i = 0; i <= n; ++i) matrix[i][0] = i * score.gap;
    for (size_t j = 0; j <= m; ++j) matrix[0][j] = j * score.gap;

    auto start = std::chrono::high_resolution_clock::now();

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

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Algorithm: Needleman-Wunsch" << std::endl;
    std::cout << "Input_Size: " << n << "x" << m << std::endl;
    std::cout << "Final_Score: " << matrix[n][m] << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
}

int main() {
    // Small test sequences
    std::string s1 = "GATTACA";
    std::string s2 = "GCATGCU";
    needleman_wunsch(s1, s2);
    return 0;
}
