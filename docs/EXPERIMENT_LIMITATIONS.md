# Experiment Limitations

This document describes the limitations of the SRF experiment and the performance measurements conducted.

## Hardware Dependence
The performance of SRF variants is highly dependent on the target hardware architecture, particularly:
- **Cache Size and Hierarchy:** Memory-time trade-offs are sensitive to cache hit rates and memory bandwidth.
- **CPU Scheduling:** System noise and OS-level scheduling jitter can affect wall-clock timing measurements.

## Measurement Proxies
While we aim for accurate measurements, we use the following proxies where direct measurement is not feasible:
- **Peak Memory Usage (RSS):** We use Peak Resident Set Size as a proxy for the algorithm's actual memory footprint.
- **Recompute Events:** We explicitly count the number of times a cell, probability, or node is recomputed to quantify the computational overhead.
- **Cache-Hits Diagnostic:** A proxy for data access density based on the total number of primary memory access operations.

## No Universal Speedup Claim
SRF is a memory-restructuring methodology, not an asymptotic improvement. While recomputation can sometimes lead to speedups due to improved cache locality or reduced page faults, this project makes no universal claim that SRF variants will be faster than their corresponding baselines.
