# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 5-A: Granularity Control — COMPLETED**
- **Atomic Unit Management:** Explicit control over recomputation granularity (Tiles, Segments, Groups).
- **Unit-Level Instrumentation:** Tracking of unit recomputation events and reuse proxies.
- **Overhead Amortization:** Characterized the relationship between unit size and architectural efficiency.
- **Multi-Backend Scale:** Granularity policies are fully integrated with CPU and optional GPU backends.

## Project History
- **Phase 1:** Baseline Characterization (Correctness & Ground Truth).
- **Phase 2:** Functional SRF (Memory Footprint Reduction).
- **Phase 3:** Performance SRF (Locality & Cache Awareness).
- **Phase 4:** Architectural SRF (Multi-Backend & Scale).
- **Phase 5-A:** Granularity SRF (Unit-Level Amortization).

## Repository Structure
- `srf/granularity/`: Granularity policy definitions and grouping logic.
- `srf/`: Backend-agnostic, locality-aware, and granularity-aware recomputation implementations.
- `docs/`: Technical references, including Phase 5-A Model and Results.
- `benchmarks/`: Granularity-aware benchmarking suite.

## Validation and Reproducibility
- **Correctness:** Bit-exact equivalence verified across all granularity unit sizes.
- **CI/CD:** Automated granularity sweeps on macOS, Linux, and Windows.
