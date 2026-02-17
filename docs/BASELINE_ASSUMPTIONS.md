# Baseline Assumptions

This document describes the assumptions and limitations of the baseline implementations in the SRF project.

## Algorithms Implemented
The following algorithms are implemented as serial, sequential baselines in C++17:
- **Needleman-Wunsch (Global Alignment):** Simple scoring model (+1 match, -1 mismatch, -1 gap).
- **Viterbi Algorithm:** Discrete HMM with 2 states and 3 observations.
- **Forward Algorithm:** Discrete HMM with the same model as Viterbi.
- **Graph DP (Shortest Path DAG):** Simple weighted DAG with 5 nodes.

## Intentional Lack of Optimization
These implementations are intentionally **unoptimized** to serve as a pure sequential baseline.
- **No Parallelism:** No threading, OpenMP, or asynchronous execution.
- **No Vectorization:** No explicit SIMD (AVX/SSE/NEON) intrinsics are used.
- **Minimal Prefetching:** Code relies on the compiler's default optimizations and cache management.
- **Standard Libraries Only:** No high-performance numerical or graph libraries are used.

## Measurement Limitations
The performance data in `results/csv/` should be interpreted with the following considerations:
- **Wall Clock Time:** Measurements use `std::chrono::high_resolution_clock`, which is susceptible to system noise and scheduling jitter.
- **Toy Models:** Baselines use small inputs and toy models to focus on algorithmic correctness and structure.
- **Compiler Optimizations:** The build script uses `-O3`, which may vary across compiler versions (GCC/Clang).

## Platform Variability Disclaimer
Measurements are expected to vary significantly across different hardware platforms and operating systems. Results recorded in CI represent a standardized but potentially non-optimal environment.
