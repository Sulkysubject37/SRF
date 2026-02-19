#ifndef SRF_CORE_UTILS_HPP
#define SRF_CORE_UTILS_HPP

#include <iostream>
#include <chrono>
#include <atomic>
#include <cmath>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/resource.h>
#endif

namespace srf {

struct Metrics {
    std::atomic<long long> recompute_events{0};
    std::atomic<long long> working_set_bytes{0};
    std::atomic<long long> tile_reuse_count{0};
    std::atomic<long long> total_dist_metric{0};
    
    // Phase 5-A: Granularity tracking
    std::atomic<long long> unit_recompute_events{0};
    std::atomic<long long> last_unit_id{-1};
    std::atomic<long long> unit_reuse_proxy{0};

    void reset() {
        recompute_events = 0;
        working_set_bytes = 0;
        tile_reuse_count = 0;
        total_dist_metric = 0;
        unit_recompute_events = 0;
        last_unit_id = -1;
        unit_reuse_proxy = 0;
    }
    void record_recompute(int count = 1) { recompute_events += count; }
    void update_working_set(size_t bytes) {
        long long current = working_set_bytes;
        while (bytes > (size_t)current && !working_set_bytes.compare_exchange_weak(current, (long long)bytes));
    }
    void record_reuse() { tile_reuse_count++; }
    void record_dist(long long d) { total_dist_metric += d; }

    void record_unit_recompute(long long unit_id) {
        long long prev = last_unit_id.exchange(unit_id);
        if (unit_id != prev) {
            unit_recompute_events++;
        } else {
            unit_reuse_proxy++;
        }
    }
};

static Metrics global_metrics;

// Lightweight Cache Model
struct CacheModel {
    size_t budget_bytes;
    size_t cell_size;

    CacheModel(size_t budget_kb, size_t cell_sz) 
        : budget_bytes(budget_kb * 1024), cell_size(cell_sz) {}

    // Suggests a TileSize such that TileSize^2 * cell_size <= budget
    int suggest_tile_size() const {
        if (budget_bytes == 0) return 20; // Default fallback
        int side = static_cast<int>(std::sqrt(budget_bytes / cell_size));
        return (side > 2) ? side : 2;
    }
};

size_t get_peak_rss() {
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS info;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info))) {
        return info.PeakWorkingSetSize / 1024;
    }
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
#if defined(__APPLE__) && defined(__MACH__)
        return usage.ru_maxrss / 1024; 
#else
        return usage.ru_maxrss;
#endif
    }
#endif
    return 0;
}

} // namespace srf

#endif