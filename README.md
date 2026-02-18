# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 4: Backend Abstraction & Scalability — COMPLETED**
- **Multi-Backend Execution:** Support for both CPU and optional GPU (simulated) backends.
- **Backend-Agnostic Logic:** SRF recomputation core is decoupled from hardware-specific primitives.
- **Scalability Study:** Performance characterized for large-scale inputs (up to $N=10000$ for Graph-DP).
- **Safe Fallback:** Guaranteed CPU execution if GPU hardware or memory is unavailable.

## Project History
- **Phase 1:** Baseline Characterization.
- **Phase 2:** Functional SRF (Memory Reduction).
- **Phase 3:** Performance SRF (Locality & Cache Awareness).
- **Phase 4:** Architectural SRF (Backend Abstraction & Scale).

## Repository Structure
- `baselines/`: Reference implementations.
- `srf/`: Locality-aware, backend-agnostic recomputation core.
    - `backends/`: Execution primitives for CPU and GPU.
    - `runtime/`: Dynamic backend selector.
- `docs/`: Results and technical references for all phases.
- `benchmarks/`: Scalability-aware benchmarking suite.
- `tests/`: Multi-backend correctness verification.

## Validation and Reproducibility
- **CI-Based Validation:** Every push is tested across macOS, Linux, and Windows.
- **Multi-Backend CI:** The CI pipeline builds both backends and falls back to CPU for execution.
