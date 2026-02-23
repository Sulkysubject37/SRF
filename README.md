# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 10: Scientific Hardening & Formalization — COMPLETED**
- **Formalized Claims:** Explicit separation of structural memory guarantees from hardware-dependent performance variables.
- **Failure Mode Mapping:** Documented regimes of Locality Collapse and Management Overhead Dominance.
- **Architectural Reference:** Complete technical history formalized in [docs/PROJECT_REFERENCE.md](docs/PROJECT_REFERENCE.md).
- **Hardened Validation:** Verified bit-exact algorithmic determinism across all platforms.

## Scientific Hardening Summary

### 1. Fundamental Guarantees
- **Memory Feasibility:** Enables genomic-scale computations (up to 1M bp) within a fixed $O(N)$ memory footprint.
- **Bit-Exact Determinism:** 100% equivalence between SRF variants and standard baselines.
- **Metric Invariance:** Algorithmic intensity ratios ($R_{mem}$, $R_{rec}$) are proven platform-independent.

### 2. Failure Regimes & Non-Guarantees
- **No Universal Speedup:** Recomputation introduces an arithmetic tax; speed is only clawed back via locality optimization.
- **Locality Collapse:** Performance degrades if dependency spans exceed physical cache capacity.
- **Management Floor:** Management logic dominates runtime for very small workloads ($N < 100$).

## Memory Savings Impact (Genomic Scales)
| Scale | Input Size | Regular Memory | SRF Memory | **Savings** |
| :--- | :---: | :---: | :---: | :--- |
| **M** | 1,000 | ~4.0 MB | ~8.0 KB | **99.8%** |
| **XL** | 4,000 | ~64.0 MB | ~32.0 KB | **99.95%** |
| **EXTREME** | 1,000,000 | **~4.0 TB** | **~8.0 MB** | **99.999%** |

---

## Repository Structure
- `srf/`: Backend-agnostic, locality-aware, and granularity-aware recomputation core.
- `control/`: Self-stabilizing drift detection and adaptation policy layer.
- `analysis/`: Deterministic algebraic models and regime mapping.
- `docs/`: Formal Technical Reference (PDF), Regime Taxonomy, and Validity Regions.

## Validation and Reproducibility
- **CI Philosophy:** Continuous correctness verification on macOS, Linux, and Windows with XS/S scale restrictions.
- **Data Integrity:** All findings derived from actual execution logs (no fitted constants).
