# Baseline Assumptions

This document describes the assumptions and limitations of the baseline implementations in the SRF project.

## Algorithms Implemented
The following algorithms are implemented as serial, sequential baselines in C++17:
- **Needleman-Wunsch (Global Alignment):** Sequential implementation with a 200x200 sequence size.
- **Viterbi Algorithm:** Discrete HMM with 2 states and a 100-step observation sequence.
- **Forward Algorithm:** Discrete HMM with the same model and observation sequence as Viterbi.
- **Graph DP (Shortest Path DAG):** 500-node DAG with a fixed dependency structure.

## Performance Measurement Strategy
To ensure measurable and stable execution times:
1.  **Multiple Iterations:** Each algorithm is executed in a loop (ranging from 100 to 1000 iterations depending on the algorithm's complexity).
2.  **Average Time:** The total execution time is divided by the number of iterations to calculate the average time per execution in microseconds (us).
3.  **Optimization Prevention:** Intermediate results are aggregated and printed at the end of the program to prevent the compiler from optimizing away the core logic.

## Intentional Lack of Optimization
These implementations are intentionally **unoptimized** to serve as a pure sequential baseline.
- **No Parallelism:** No threading, OpenMP, or asynchronous execution.
- **No Vectorization:** No explicit SIMD (AVX/SSE/NEON) intrinsics are used.
- **Minimal Prefetching:** Code relies on the compiler's default optimizations (`-O3`).

## Measurement Limitations
The performance data should be interpreted with the following considerations:
- **Wall Clock Time:** Measurements use `std::chrono::high_resolution_clock`.
- **Stability:** Averaging across iterations reduces noise but still reflects system-level variability.
- **Platform Variability:** Results will vary significantly between Linux, macOS, and Windows environments.