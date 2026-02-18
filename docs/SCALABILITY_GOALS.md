# Phase 4 Scalability Goals

## Objective
The goal of the Phase 4 scalability study is to characterize SRF behavior at problem sizes that approach realistic bioinformatics scales (e.g., $N=1000$ for alignment, 5000 steps for HMMs).

## Regime Transitions
- **Compute-Bound Regime:** At larger scales, the cost of recomputation events scales linearly, while the memory footprint remains constant ($O(N)$). 
- **Overhead Amortization:** As input size increases, the constant overhead of kernel launches and data transfers is expected to be amortized, allowing the potential benefits of acceleration to become measurable.

## Measurement Caveats
Large-scale runs are more susceptible to system noise and OS jitter. All scalability results are averaged over multiple iterations to ensure stability. Memory usage is tracked per-platform to identify "RSS Blowup" points where OS-level page management overrides algorithmic logic.
