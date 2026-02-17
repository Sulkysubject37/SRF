# Experimental Protocol

This document outlines the methodology for benchmarking and validating the Structured Recomputation Framework (SRF).

## Study Objective
The objective of this study is to characterize the baseline performance and memory usage of standard dynamic programming and graph-based algorithms. This baseline will serve as a reference point for future research into recomputation strategies.

## Algorithms Under Test
1. **Needleman-Wunsch:** Global sequence alignment.
2. **Viterbi Algorithm:** Finding the most likely sequence of hidden states in an HMM.
3. **Forward Algorithm:** Calculating the total likelihood of an observed sequence in an HMM.
4. **Graph DP:** Shortest path or dependency evaluation on a Directed Acyclic Graph (DAG).

## Metrics Recorded
Measurements are performed during each benchmark run and recorded in `results/csv/benchmark_log.csv`:
- **Wall Clock Time:** Measured using `<chrono>` in C++ for the core execution loop.
- **Deterministic Output:** Verified against fixed inputs to ensure correctness.

## Reproducibility Statement
To ensure reproducibility:
- All benchmarks are executed using the scripts in `benchmarks/`.
- The build environment uses standard C++17 flags.
- CI workflows in `.github/workflows/` define the exact environment for official measurements.
- Raw data is stored in CSV format for transparency.

## No Performance Claims
This protocol is designed for baseline characterization only. No claims of speedup, optimization, or relative efficiency are made at this phase of the project.