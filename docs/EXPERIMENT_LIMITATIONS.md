# Experiment Limitations

This document describes the limitations of the SRF experiment and the performance measurements conducted.

## Hardware Dependence
The performance of SRF variants is highly dependent on the target hardware architecture, particularly:
- **Cache Size and Hierarchy:** Memory-time trade-offs are sensitive to cache hit rates and memory bandwidth.
- **CPU Scheduling:** System noise and OS-level scheduling jitter can affect wall-clock timing measurements.

## Hardware Counter Availability
Direct measurement of hardware events (e.g., L1 cache misses via PAPI or perf_event) is not consistently available across all platforms (macOS/Linux/Windows) in the CI environment.

## Proxy Metric Disclaimers
We use the following proxies for Phase 3 studies:
- **Working Set Proxy:** Calculated based on the size of active DP tables and checkpoints.
- **Locality Proxy:** Calculated as the distance in the evaluation schedule between a node and its dependencies.
- **Recompute Events:** Counted explicitly to measure computational overhead.

## No Universal Speedup Claim
SRF is a memory-restructuring methodology, not an asymptotic improvement. Phase 3 optimizations are aimed at constant-factor performance gains within a fixed memory budget.
