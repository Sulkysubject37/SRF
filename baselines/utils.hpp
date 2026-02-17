#ifndef SRF_UTILS_HPP
#define SRF_UTILS_HPP

#include <iostream>
#include <chrono>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/resource.h>
#endif

namespace srf {

// Returns peak resident set size in Kilobytes
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
        return usage.ru_maxrss / 1024; // macOS returns bytes
#else
        return usage.ru_maxrss; // Linux returns kilobytes
#endif
    }
#endif
    return 0;
}

} // namespace srf

#endif
