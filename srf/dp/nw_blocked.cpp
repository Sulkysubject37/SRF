#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include "../core/srf_utils.hpp"
#include "../runtime/backend_selector.h"
#include "../granularity/granularity_policy.h"

struct Scoring {
    int match = 1;
    int mismatch = -1;
    int gap = -1;
};

int nw_granularity_aware(const std::string& s1, const std::string& s2, int B, int G, int cache_budget_kb, const Scoring& score, srf::IBackend* backend) {
    size_t n = s1.length();
    size_t m = s2.length();
    
    // B is the checkpoint/tile boundary from Phase 3
    // G is the granularity unit size from Phase 5-A
    srf::GranularityPolicy policy(srf::GranularityType::TILE, G);
    
    std::vector<int> prev(m + 1), curr(m + 1);
    srf::global_metrics.update_working_set((prev.size() + curr.size()) * sizeof(int));

    for (size_t j = 0; j <= m; ++j) prev[j] = j * score.gap;

    for (size_t i = 1; i <= n; ++i) {
        curr[0] = i * score.gap;
        for (size_t j = 1; j <= m; ++j) {
            int match_score = (s1[i - 1] == s2[j - 1]) ? score.match : score.mismatch;
            
            curr[j] = backend->nw_cell_compute(prev[j - 1], prev[j], curr[j - 1], match_score, score.gap);
            
            // Logic: If we are not on a checkpoint boundary (B), we recompute.
            // Tracking: We record the recompute event at the granularity of G.
            if (i % B != 0 && j % B != 0) {
                srf::global_metrics.record_recompute(1);
                srf::global_metrics.record_unit_recompute(policy.get_unit_id_2d(i, j));
            } else {
                srf::global_metrics.record_reuse();
            }
        }
        prev = curr;
    }
    return prev[m];
}

int main(int argc, char* argv[]) {
    // Mapping: ./build/bin [SeqLen] [B] [G] [GPUBudget]
    int seq_len = (argc > 1) ? std::stoi(argv[1]) : 300;
    int B = (argc > 2) ? std::stoi(argv[2]) : 20;
    int G = (argc > 3) ? std::stoi(argv[3]) : 1;
    size_t gpu_budget = (argc > 4) ? std::stoul(argv[4]) : 1024;

    auto backend = srf::BackendSelector::select(gpu_budget);

    std::string s1(seq_len, 'A');
    std::string s2(seq_len, 'T');
    for(int i=0; i<seq_len; i+=5) s1[i] = 'T';

    Scoring score;
    srf::global_metrics.reset();
    backend->reset_metrics();
    
    auto start = std::chrono::high_resolution_clock::now();
    // Cache budget set to 0 to prioritize explicit B/G study
    int result = nw_granularity_aware(s1, s2, B, G, 0, score, backend.get());
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    auto b_metrics = backend->get_metrics();

    std::cout << "Algorithm: Needleman-Wunsch" << std::endl;
    std::cout << "Variant: SRF-GranularityAware" << std::endl;
    std::cout << "Backend: " << (backend->type() == srf::BackendType::GPU ? "gpu" : "cpu") << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    std::cout << "Recompute_Events: " << srf::global_metrics.recompute_events << std::endl;
    std::cout << "Unit_Recompute_Events: " << srf::global_metrics.unit_recompute_events << std::endl;
    std::cout << "Unit_Reuse_Proxy: " << srf::global_metrics.unit_reuse_proxy << std::endl;
    std::cout << "Granularity_Unit_Size: " << G << std::endl;
    std::cout << "Working_Set_Proxy: " << srf::global_metrics.working_set_bytes << std::endl;
    std::cout << "Transfer_Overhead_us: " << b_metrics.transfer_overhead_us << std::endl;
    std::cout << "Kernel_Launch_Count: " << b_metrics.kernel_launch_count << std::endl;
    std::cout << "Param_1: " << B << std::endl;
    std::cout << "Param_2: " << seq_len << std::endl;
    std::cout << "Param_3: " << G << std::endl;

    return 0;
}