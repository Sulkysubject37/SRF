# Phase 1: Baseline Performance Characterization

This document provides a transparent summary of the baseline performance, memory usage, and cache diagnostics across three major operating systems.

## Summary Table (Average Results)

| Algorithm | Platform | Time (µs) | Memory (KB) | Cache Diag (Accesses) |
| :--- | :--- | :---: | :---: | :---: |
| **Needleman-Wunsch** | Darwin | 158.5 | 4480 | 90000 |
| **Forward** | Darwin | 16.5 | 4528 | 2000 |
| **Viterbi** | Darwin | 16.4 | 4544 | 2000 |
| **Graph-DP** | Darwin | 5.2 | 4432 | 1000 |

## Metrics Description
- **Time (µs):** Wall-clock time averaged over 50-500 iterations.
- **Memory (KB):** Peak Resident Set Size (RSS) during execution.
- **Cache Diag:** A proxy for data access density (Total DP table cell visits).

## Visual Comparison
### Master Performance Profiles
The following master plots provide a consolidated view of all measured parameters (Runtime, Memory, and Cache Accesses) for each platform:

- **macOS (Darwin):** ![Darwin Master Plot](../results/plots/master_profile_darwin.png)
- **Linux:** ![Linux Master Plot](../results/plots/master_profile_linux.png)
- **Windows:** ![Windows Master Plot](../results/plots/master_profile_mingw64_nt-10.0-26100.png)

## Data Integrity
- **Methodology:** Peak RSS tracked via `getrusage` (Unix) and `GetProcessMemoryInfo` (Windows).
- **Raw Data:** See `results/csv/benchmark_log.csv`.