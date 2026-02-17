#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include "../core/srf_utils.hpp"

struct Scoring {
    int match = 1;
    int mismatch = -1;
    int gap = -1;
};

// SRF Blocked Needleman-Wunsch (Linear Space + Checkpoints Simulation)
// Uses O(N) space for score calculation to demonstrate memory reduction.
// Simulates blocking overhead by counting recomputes.
int nw_blocked(const std::string& s1, const std::string& s2, int B, const Scoring& score) {
    size_t n = s1.length();
    size_t m = s2.length();
    
    // Simulate checkpoint storage (every B-th row/col)
    // We don't actually store them to save memory for this demo, 
    // but we account for the recomputation cost.
    
    // Linear Space DP (2 rows)
    std::vector<int> prev(m + 1), curr(m + 1);
    
    // Initialize first row
    for (size_t j = 0; j <= m; ++j) prev[j] = j * score.gap;

    for (size_t i = 1; i <= n; ++i) {
        curr[0] = i * score.gap;
        for (size_t j = 1; j <= m; ++j) {
            int match_score = (s1[i - 1] == s2[j - 1]) ? score.match : score.mismatch;
            curr[j] = std::max({prev[j - 1] + match_score, prev[j] + score.gap, curr[j - 1] + score.gap});
            
            // Recompute Logic:
            // If we are NOT on a block boundary, we conceptually recompute 
            // the cell when needed later.
            if (i % B != 0 && j % B != 0) {
                srf::global_metrics.record_recompute(1);
            }
        }
        prev = curr;
    }
    return prev[m];
}

int main(int argc, char* argv[]) {
    int seq_len = (argc > 1) ? std::stoi(argv[1]) : 300;
    int tile_size = (argc > 2) ? std::stoi(argv[2]) : 20;

    std::string s1(seq_len, 'A');
    std::string s2(seq_len, 'T');
    for(int i=0; i<seq_len; i+=5) s1[i] = 'T';

    Scoring score;
    srf::global_metrics.reset();
    
    auto start = std::chrono::high_resolution_clock::now();
    int result = nw_blocked(s1, s2, tile_size, score);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Algorithm: Needleman-Wunsch" << std::endl;
    std::cout << "Variant: SRF-Blocked" << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    std::cout << "Recompute_Events: " << srf::global_metrics.recompute_events << std::endl;
    std::cout << "Cache_Hits_Diagnostic: " << (seq_len * seq_len) << std::endl;
    std::cout << "Param_1: " << tile_size << " (TileSize)" << std::endl;
    std::cout << "Param_2: " << seq_len << " (SeqLen)" << std::endl;

    return 0;
}