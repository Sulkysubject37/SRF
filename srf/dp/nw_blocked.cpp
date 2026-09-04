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
    if (n == 0 || m == 0) return 0;

    int B = B_init; // Fixed blocking factor for grid stability
    srf::GranularityPolicy policy(srf::GranularityType::TILE, G);
    srf::DriftDetector detector;
    srf::AdaptationPolicy adapter(50);

    std::vector<int> prev(m + 1), curr(m + 1);

    // Sparse checkpoint buffers: O(NM/B) memory
    size_t h_size = n / B + 2;
    size_t v_size = m / B + 2;
    std::vector<std::vector<int>> H_bounds(h_size, std::vector<int>(m + 1, 0));
    std::vector<std::vector<int>> V_bounds(v_size, std::vector<int>(n + 1, 0));

    srf::global_metrics.update_working_set((prev.size() * 2 + h_size * (m+1) + v_size * (n+1)) * sizeof(int));

    // Initialize Boundaries
    for (size_t j = 0; j <= m; ++j) {
        prev[j] = j * -1;
        H_bounds[0][j] = prev[j];
        srf::global_metrics.record_mem_access();
    }
    for (size_t i = 0; i <= n; ++i) {
        V_bounds[0][i] = i * -1;
    }

    // 1. FORWARD PASS
    for (size_t i = 1; i <= n; ++i) {
        curr[0] = i * -1;
        srf::global_metrics.record_mem_access();
        
        for (size_t j = 1; j <= m; ++j) {
            int match_score = (s1[i - 1] == s2[j - 1]) ? 1 : -1;
            curr[j] = backend->nw_cell_compute(prev[j - 1], prev[j], curr[j - 1], match_score, -1);
            srf::global_metrics.record_compute(1);
            srf::global_metrics.record_mem_access();

            // Save vertical boundaries
            if (j % B == 0) {
                V_bounds[j / B][i] = curr[j];
            }
        }
        
        // Save horizontal boundaries
        if (i % B == 0) {
            H_bounds[i / B] = curr;
        }
        prev = curr;

        if (i % 100 == 0) {
            observer.record_snapshot(srf::global_metrics.compute_events, 
                                     srf::global_metrics.recompute_events, 
                                     srf::global_metrics.memory_access_proxy, 
                                     srf::global_metrics.working_set_bytes);
            // Simulate drift detection without mutating B for strict grid integrity
            detector.detect(observer);
        }
    }

    int final_score = prev[m];

    // Final boundary persistence for incomplete bottom/right edges
    if (n % B != 0) {
        H_bounds[n / B + 1] = prev;
    }
    
    // 2. BACKWARD PASS (Traceback with On-Demand Tile Recomputation)
    size_t i = n, j = m;
    std::vector<std::pair<int, int>> alignment_path;

    while (i > 0 || j > 0) {
        if (i == 0) { j--; alignment_path.push_back({0, 1}); continue; }
        if (j == 0) { i--; alignment_path.push_back({1, 0}); continue; }

        // Determine current tile indices
        size_t I = (i - 1) / B;
        size_t J = (j - 1) / B;

        size_t start_i = I * B;
        size_t start_j = J * B;
        
        size_t end_i = std::min(start_i + B, n);
        size_t end_j = std::min(start_j + B, m);

        size_t local_h = end_i - start_i;
        size_t local_w = end_j - start_j;

        std::vector<std::vector<int>> local_M(local_h + 1, std::vector<int>(local_w + 1, 0));
        srf::global_metrics.update_working_set(srf::global_metrics.working_set_bytes + (local_h+1)*(local_w+1)*sizeof(int));

        // Load boundaries for the localized tile
        for (size_t lc = 0; lc <= local_w; ++lc) {
            local_M[0][lc] = H_bounds[I][start_j + lc];
        }
        for (size_t lr = 0; lr <= local_h; ++lr) {
            local_M[lr][0] = V_bounds[J][start_i + lr];
        }

        // Recomputation Kernel for the local tile
        for (size_t lr = 1; lr <= local_h; ++lr) {
            for (size_t lc = 1; lc <= local_w; ++lc) {
                int match_score = (s1[start_i + lr - 1] == s2[start_j + lc - 1]) ? 1 : -1;
                local_M[lr][lc] = backend->nw_cell_compute(local_M[lr-1][lc-1], local_M[lr-1][lc], local_M[lr][lc-1], match_score, -1);
                
                // ACTUAL RECOMPUTATION TRACKING
                srf::global_metrics.record_recompute(1);
                srf::global_metrics.record_unit_recompute(policy.get_unit_id_2d(start_i + lr, start_j + lc));
            }
        }

        // Local Traceback inside the reconstructed tile
        size_t curr_lr = i - start_i;
        size_t curr_lc = j - start_j;

        while (curr_lr > 0 && curr_lc > 0) {
            int score = local_M[curr_lr][curr_lc];
            int diag = local_M[curr_lr-1][curr_lc-1];
            int up = local_M[curr_lr-1][curr_lc];
            
            int match_score = (s1[start_i + curr_lr - 1] == s2[start_j + curr_lc - 1]) ? 1 : -1;

            if (score == diag + match_score) {
                i--; j--; curr_lr--; curr_lc--;
                alignment_path.push_back({1, 1});
            } else if (score == up - 1) {
                i--; curr_lr--;
                alignment_path.push_back({1, 0});
            } else {
                j--; curr_lc--;
                alignment_path.push_back({0, 1});
            }
        }
    }

    // Final snapshot to capture the recomputation overhead during traceback
    observer.record_snapshot(srf::global_metrics.compute_events, 
                             srf::global_metrics.recompute_events, 
                             srf::global_metrics.memory_access_proxy, 
                             srf::global_metrics.working_set_bytes);

    return final_score;
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
