# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 3: Locality & Fixed-Memory Runtime Study — COMPLETED**
- **Locality-Aware Variants:** Implemented cache-aware tiling and deterministic scheduling strategies.
- **Fixed-Memory Runtime:** Verified that runtime can be optimized within a static memory budget through better cache management.
- **Master Analysis:** Final cross-platform results available in [docs/PHASE_3_RESULTS.md](docs/PHASE_3_RESULTS.md).

## Project History
- **Phase 1:** Baseline Characterization (Correctness & Standard Metrics).
- **Phase 2:** Functional SRF (Memory Reduction via Recomputation).
- **Phase 3:** Performance SRF (Locality & Cache Awareness).

## Repository Structure
- `baselines/`: Reference implementations.
- `srf/`: Locality-aware recomputation implementations.
- `docs/`: Phase 1, 2, & 3 Results and Theoretical Models.
- `benchmarks/`: Integrated benchmarking suite.
- `tests/`: Continuous correctness verification.
- `results/`: Platform-specific CI artifacts.

## Validation and Reproducibility
- **CI-Based Validation:** Every push is tested against macOS, Linux, and Windows runners.
- **Metric Integrity:** All data generated from actual execution runs (no fabricated data).