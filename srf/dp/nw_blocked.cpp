#include <iostream>
#include <vector>
#include <string>
#include <fstream>
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

std::string load_file(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return "";
    std::string s;
    f >> s;
    return s;
}

int nw_granularity_aware(const std::string& s1, const std::string& s2, int B, int G, srf::IBackend* backend) {
    size_t n = s1.length();
    size_t m = s2.length();
    srf::GranularityPolicy policy(srf::GranularityType::TILE, G);
    std::vector<int> prev(m + 1), curr(m + 1);
    srf::global_metrics.update_working_set((prev.size() + curr.size()) * sizeof(int));

    for (size_t j = 0; j <= m; ++j) {
        prev[j] = j * -1; // gap
        srf::global_metrics.record_mem_access();
    }

    for (size_t i = 1; i <= n; ++i) {
        curr[0] = i * -1;
        srf::global_metrics.record_mem_access();
        for (size_t j = 1; j <= m; ++j) {
            int match_score = (s1[i - 1] == s2[j - 1]) ? 1 : -1;
            curr[j] = backend->nw_cell_compute(prev[j - 1], prev[j], curr[j - 1], match_score, -1);
            srf::global_metrics.record_compute(1);
            srf::global_metrics.record_mem_access();
            srf::global_metrics.record_mem_access();
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
    // Usage: ./nw_blocked [PathA] [PathB] [B] [G] [ScaleLabel]
    if (argc < 5) return 1;
    std::string s1 = load_file(argv[1]);
    std::string s2 = load_file(argv[2]);
    int B = std::stoi(argv[3]);
    int G = std::stoi(argv[4]);
    std::string scale = (argc > 5) ? argv[5] : "NA";

    auto backend = srf::BackendSelector::select(1024);
    srf::global_metrics.reset();
    backend->reset_metrics();
    
    auto start = std::chrono::high_resolution_clock::now();
    int result = nw_granularity_aware(s1, s2, B, G, backend.get());
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::microseconds(std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count();
    auto b_metrics = backend->get_metrics();

    std::cout << "Algorithm: Needleman-Wunsch" << std::endl;
    std::cout << "Dataset_Scale: " << scale << std::endl;
    std::cout << "Input_Size: " << s1.length() << std::endl;
    std::cout << "Backend: " << (backend->type() == srf::BackendType::GPU ? "gpu" : "cpu") << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    std::cout << "Recompute_Events: " << srf::global_metrics.recompute_events << std::endl;
    std::cout << "Compute_Events: " << srf::global_metrics.compute_events << std::endl;
    std::cout << "Memory_Access_Proxy: " << srf::global_metrics.memory_access_proxy << std::endl;
    std::cout << "Working_Set_Proxy: " << srf::global_metrics.working_set_bytes << std::endl;
    std::cout << "Granularity_Unit_Size: " << G << std::endl;
    std::cout << "Unit_Recompute_Events: " << srf::global_metrics.unit_recompute_events << std::endl;
    std::cout << "Unit_Reuse_Proxy: " << srf::global_metrics.unit_reuse_proxy << std::endl;
    std::cout << "Dispatch_Overhead_Proxy: " << srf::global_metrics.dispatch_overhead_proxy << std::endl;
    std::cout << "Param_1: " << B << std::endl;
    std::cout << "Param_3: " << G << std::endl;

    return 0;
}