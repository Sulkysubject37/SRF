# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 4: Backend Abstraction & Scalability — COMPLETED**
- **Multi-Backend Execution:** Seamless support for CPU and optional GPU backends.
- **High Scalability:** Verified performance up to $N=10000$ nodes for Graph-DP.
- **Global Master Study:** Final cross-platform performance profile available in [docs/PHASE_4_RESULTS.md](docs/PHASE_4_RESULTS.md).

## Project History
- **Phase 1:** Baseline Characterization (Correctness & Ground Truth).
- **Phase 2:** Functional SRF (Memory Footprint Reduction).
- **Phase 3:** Performance SRF (Locality & Cache Awareness).
- **Phase 4:** Architectural SRF (Multi-Backend & Scale).

## Repository Structure
- `baselines/`: Original $O(NM)$ reference implementations.
- `srf/`: Backend-agnostic, locality-aware recomputation implementations.
- `docs/`: Phase Results (1-4), Models, and Technical References.
- `benchmarks/`: Multi-backend, scalability-aware benchmarking suite.
- `tests/`: Bit-exact equivalence verification.

## Validation and Reproducibility
- **CI-Based Validation:** Every push is verified on macOS, Linux, and Windows.
- **Cross-Platform Integrity:** All measurements are derived from actual execution runs on standard CI environments.