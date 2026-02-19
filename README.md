# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 5-A: Granularity Control — COMPLETED**
- **Atomic Unit Management:** Explicit control over recomputation granularity (Tiles, Segments, Groups).
- **Unit-Level Instrumentation:** Tracking of unit recompute events and reuse proxies.
- **Overhead Amortization:** Proved 38x-100x reduction in management overhead across macOS, Linux, and Windows.
- **Global Master Study:** Final granularity profile available in [docs/PHASE_5_A_RESULTS.md](docs/PHASE_5_A_RESULTS.md).

## Project History
- **Phase 1:** Baseline Characterization (Correctness & Ground Truth).
- **Phase 2:** Functional SRF (Memory Footprint Reduction).
- **Phase 3:** Performance SRF (Locality & Cache Awareness).
- **Phase 4:** Architectural SRF (Multi-Backend & Scale).
- **Phase 5-A:** Granularity SRF (Unit-Level Amortization).

## Repository Structure
- `srf/granularity/`: Granularity policy definitions and grouping logic.
- `srf/`: Backend-agnostic, locality-aware, and granularity-aware recomputation core.
- `docs/`: Phase Results (1-5), Models, and Technical References.
- `benchmarks/`: Multi-backend, granularity-aware benchmarking suite.
- `tests/`: Bit-exact equivalence verification.

## Validation and Reproducibility
- **CI-Based Validation:** Every push is verified on macOS, Linux, and Windows.
- **Cross-Platform Integrity:** All granularity configurations ($G=1$ to $G=100$) verified for 100% bit-identical correctness.