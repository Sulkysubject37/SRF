# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 7: Extreme-Scale Behaviour & Regime Mapping — COMPLETED**
- **Extreme Scaling:** Verified performance for inputs up to 1M sequence length and 250k graph nodes.
- **Regime Mapping:** Characterized transitions between Memory-Bound, Balanced, and Recomputation-Dominated states.
- **Massive Space Savings:** Achieved a **99.999% reduction** in algorithmic memory state at the 1M scale.
- **Global Master Study:** Final scaling profile available in `docs/phase_7_global_master.png`.

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

## Repository Structure
- `srf/`: Recomputation core (Locality & Granularity aware).
- `analysis/`: Algebraic models and regime classification logic.
- `datasets_extreme/`: Large-scale synthetic datasets for failure-mode analysis.
- `docs/`: Technical Reference (PDF), Phase Results (1-7), and Master Plots.

## Validation and Reproducibility
- **CI Safety:** Automated moderate-scale stress testing on every push with extreme fallback.
- **Deterministic:** 100% bit-exact correctness verified across all scales and platforms.