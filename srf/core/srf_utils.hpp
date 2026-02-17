#ifndef SRF_CORE_UTILS_HPP
#define SRF_CORE_UTILS_HPP

#include <iostream>
#include <chrono>
#include <atomic>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/resource.h>
#endif

namespace srf {

struct Metrics {
    std::atomic<long long> recompute_events{0};
    
    void reset() { recompute_events = 0; }
    void record_recompute(int count = 1) { recompute_events += count; }
};

static Metrics global_metrics;

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
