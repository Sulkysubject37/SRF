# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 7: Extreme-Scale Behaviour & Regime Mapping — COMPLETED**
- **Extreme Scaling:** Verified performance for inputs up to 1M sequence length and 250k graph nodes.
- **Regime Mapping:** Characterized transitions between Compute-Bound, Memory-Bound, and Recomputation-Dominated states.
- **Complexity Preservation:** Confirmed algorithmic stability at the extreme limits of memory-constrained hardware.
- **Stress Reports:** Full regime characterization available in `results/csv/regime_mapping.csv`.

## Project History
- **Phase 1-3:** Baseline to Performance (Locality & Cache).
- **Phase 4:** Architectural Abstraction (Multi-Backend).
- **Phase 5:** Granularity & Analytical Modeling.
- **Phase 6:** Biological Workload Characterization.
- **Phase 7:** Extreme-Scale Stress Testing.

## Memory Savings Impact (Genomic Scales)
| Scale | Seq Length | Regular Memory | SRF Memory | **Savings** |
| :--- | :---: | :---: | :---: | :--- |
| **M** | 1,000 | ~4.0 MB | ~8.0 KB | **99.8%** |
| **XL** | 4,000 | ~64.0 MB | ~32.0 KB | **99.95%** |
| **EXTREME** | 1,000,000 | **~4.0 TB** | **~8.0 MB** | **99.999%** |

## Repository Structure
- `srf/`: Recomputation core (Locality & Granularity aware).
- `analysis/`: Algebraic models and regime classification logic.
- `stress_tests/`: Moderate and Extreme scale benchmarking suites.
- `datasets_extreme/`: Large-scale synthetic datasets for failure-mode analysis.

## Validation and Reproducibility
- **CI Safety:** Automated moderate-scale stress testing on every push.
- **Deterministic:** All results verified for bit-exact correctness at all scales.
