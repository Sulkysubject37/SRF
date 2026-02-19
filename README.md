# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 5-B: Analytical Modeling — COMPLETED**
- **Deterministic Explanatory Models:** Algebraic derivations for cost, cache-fit, and accelerator feasibility.
- **Predictive Analysis Layer:** Automated derivation of architectural indicators from observable metrics.
- **Model-Driven Visualization:** Trend analysis contrasting model predictions with empirical runtime floors.
- **Scientific Auditability:** Documented modeling philosophy and validation limitations.

## Project History
- **Phase 1:** Baseline Characterization.
- **Phase 2:** Functional SRF (Memory Footprint Reduction).
- **Phase 3:** Performance SRF (Locality & Cache Awareness).
- **Phase 4:** Architectural SRF (Multi-Backend & Scale).
- **Phase 5-A:** Granularity SRF (Unit-Level Amortization).
- **Phase 5-B:** Analytical SRF (Algebraic Explanations).

## Repository Structure
- `analysis/`: Deterministic models and predictive analysis driver.
- `srf/`: Backend-agnostic, locality-aware, and granularity-aware recomputation core.
- `docs/`: Phase Results (1-5), Model Definitions, and Technical References.
- `benchmarks/`: Multi-backend, granularity-aware benchmarking suite.

## Validation and Reproducibility
- **Model Integrity:** All analytical results are derived from raw CSV metrics via `predictive_analysis.py`.
- **CI/CD:** Automated model execution and validation on macOS, Linux, and Windows.
