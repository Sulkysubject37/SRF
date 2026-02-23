# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 8: Regime Stability & Adaptive Policies — COMPLETED**
- **Self-Stabilizing Core:** Dynamic detection of execution regime drift using deterministic ratios.
- **Conservative Adaptation:** Bounded, reversible parameter adjustments governed by strict stability guards.
- **Platform Invariance:** Verified 100% bit-identical $R_{mem}$ and $R_{rec}$ metrics across macOS, Linux, and Windows.
- **Global Stability Study:** Final cross-platform stability profile in [docs/phase_8_global_master.png](docs/phase_8_global_master.png).

## Memory Savings Impact (Genomic Scales)
| Scale | Input Size | Regular Memory | SRF Memory | **Savings** |
| :--- | :---: | :---: | :---: | :--- |
| **M** | 1,000 | ~4.0 MB | ~8.0 KB | **99.8%** |
| **XL** | 4,000 | ~64.0 MB | ~32.0 KB | **99.95%** |
| **EXTREME** | 1,000,000 | **~4.0 TB** | **~8.0 MB** | **99.999%** |

## Project History
- **Phase 1-3:** Baseline to Performance (Locality & Cache).
- **Phase 4:** Architectural Abstraction (Multi-Backend).
- **Phase 5:** Granularity & Analytical Modeling.
- **Phase 6:** Biological Workload Characterization.
- **Phase 7:** Extreme-Scale Stress Testing.
- **Phase 8:** Self-Stabilizing Regime Control.

## Repository Structure
- `srf/`: Recomputation core (Locality, Granularity, and Stability aware).
- `control/`: Deterministic drift detection and adaptation policy layer.
- `analysis/`: Algebraic models and regime classification logic.
- `docs/`: Technical Reference (PDF), All Phase Results, and Master Plots.

## Validation and Reproducibility
- **Metric Determinism:** All stability indicators are derived from bit-exact algorithmic counters.
- **Continuous Verification:** Every push is tested across macOS, Linux, and Windows for correctness and stability.
