#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "../core/srf_utils.hpp"
#include "../runtime/backend_selector.h"
#include "../granularity/granularity_policy.h"
#include "../control/drift_detector.h"
#include "../control/adaptation_policy.h"

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

int nw_granularity_aware(const std::string& s1, const std::string& s2, int B_init, int G, srf::IBackend* backend, srf::RegimeObserver& observer) {
    size_t n = s1.length();
    size_t m = s2.length();
    srf::GranularityPolicy policy(srf::GranularityType::TILE, G);
    std::vector<int> prev(m + 1), curr(m + 1);
    srf::global_metrics.update_working_set((prev.size() + curr.size()) * sizeof(int));

    int current_B = B_init;
    srf::DriftDetector detector;
    srf::AdaptationPolicy adapter(50); // Cooldown

    for (size_t j = 0; j <= m; ++j) {
        prev[j] = j * -1;
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
            if (i % current_B != 0 && j % current_B != 0) {
                srf::global_metrics.record_recompute(1);
                srf::global_metrics.record_unit_recompute(policy.get_unit_id_2d(i, j));
            } else {
                srf::global_metrics.record_reuse();
            }
        }
        prev = curr;
        
        observer.record_snapshot(srf::global_metrics.compute_events, 
                                 srf::global_metrics.recompute_events, 
                                 srf::global_metrics.memory_access_proxy, 
                                 srf::global_metrics.working_set_bytes);
                                 
        // Evaluate Adaptation
        srf::DriftState ds = detector.detect(observer);
        srf::AdaptationSignal signal = adapter.evaluate(ds, observer);
        if (signal.should_adapt) {
            int old_B = current_B;
            current_B += signal.delta;
            if (current_B < 2) current_B = 2; // Stability floor
            std::cout << "ADAPTATION_EVENT: true" << std::endl;
            std::cout << "ADAPTATION_REASON: " << signal.reason << std::endl;
            std::cout << "OLD_PARAM: " << old_B << std::endl;
            std::cout << "NEW_PARAM: " << current_B << std::endl;
        }
    }
    return prev[m];
}

int main(int argc, char* argv[]) {
    if (argc < 5) return 1;
    std::string s1 = load_file(argv[1]);
    std::string s2 = load_file(argv[2]);
    int B = std::stoi(argv[3]);
    int G = std::stoi(argv[4]);
    std::string scale = (argc > 5) ? argv[5] : "NA";

    auto backend = srf::BackendSelector::select(1024);
    srf::global_metrics.reset();
    backend->reset_metrics();
    
    srf::RegimeObserver observer;

    auto start = std::chrono::high_resolution_clock::now();
    int result = nw_granularity_aware(s1, s2, B, G, backend.get(), observer);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    srf::DriftDetector detector;
    srf::DriftState d_state = detector.detect(observer);
    srf::RegimeSnapshot latest = observer.get_latest();

    std::cout << "Algorithm: Needleman-Wunsch" << std::endl;
    std::cout << "Dataset_Scale: " << scale << std::endl;
    std::cout << "Backend: " << (backend->type() == srf::BackendType::GPU ? "gpu" : "cpu") << std::endl;
    std::cout << "Result_Check: " << result << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
    std::cout << "Memory_kb: " << srf::get_peak_rss() << std::endl;
    std::cout << "Drift_State: " << (d_state == srf::DriftState::STABLE ? "STABLE" : (d_state == srf::DriftState::DRIFT_CANDIDATE ? "DRIFT_CANDIDATE" : "INSUFFICIENT_DATA")) << std::endl;
    std::cout << "R_mem: " << latest.r_mem << std::endl;
    std::cout << "R_rec: " << latest.r_rec << std::endl;
    std::cout << "Param_1: " << B << std::endl;
    std::cout << "Param_3: " << G << std::endl;

    return 0;
}
