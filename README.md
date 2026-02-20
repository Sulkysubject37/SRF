# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 6: Biological Workload Stress & Scaling — COMPLETED**
- **Real Biological Data:** Characterized against Human/Neanderthal mitochondrial genomes and the Gene Ontology (GO) hierarchy.
- **Complexity Characterization:** Verified $O(N^2)$ and $O(N)$ scaling regimes under recomputation.
- **Massive Space Savings:** Achieved up to 99.9% reduction in algorithmic memory state.
- **Global Master Study:** Final scaling profile available in [docs/PHASE_6_RESULTS.md](docs/PHASE_6_RESULTS.md).

## Memory Savings Impact (XS to XL Scales)

The primary achievement of SRF is the decoupling of algorithmic memory from input size.

### 1. Needleman-Wunsch (Alignment)
| Scale | Seq Length | Regular Memory ($O(N^2)$) | SRF Memory ($O(N)$) | **Space Savings** |
| :--- | :---: | :---: | :---: | :--- |
| **XS** | 200 | ~160 KB | ~1.6 KB | **99%** |
| **M** | 1,000 | ~4.0 MB | ~8.0 KB | **99.8%** |
| **XL** | 4,000 | **~64.0 MB** | **~32.0 KB** | **99.95%** |

### 2. HMM Inference (Forward / Viterbi)
| Scale | Seq Length | Regular Memory ($O(T \cdot S)$) | SRF Memory ($O(T/K \cdot S)$) | **Space Savings** |
| :--- | :---: | :---: | :---: | :--- |
| **XS** | 200 | ~3.2 KB | ~0.16 KB | **95%** |
| **XL** | 4,000 | **~64.0 KB** | **~3.20 KB** | **95%** |

### 3. Graph-DP (DAG Evaluation)
| Scale | Node Count | Regular Memory ($O(N)$) | SRF Memory ($O(Width)$) | **Space Savings** |
| :--- | :---: | :---: | :---: | :--- |
| **XS** | 500 | ~2.0 KB | ~0.2 KB | **90%** |
| **XL** | 10,000 | **~40.0 KB** | **~4.0 KB** | **90%** |

---

## Project History
- **Phase 1:** Baseline Characterization (Correctness & Ground Truth).
- **Phase 2:** Functional SRF (Memory Footprint Reduction).
- **Phase 3:** Performance SRF (Locality & Cache Awareness).
- **Phase 4:** Architectural SRF (Multi-Backend & Scale).
- **Phase 5:** Granularity Control & Analytical Modeling.
- **Phase 6:** Biological Workload Characterization.

## Repository Structure
- `srf/`: Backend-agnostic, locality-aware, and granularity-aware recomputation core.
- `analysis/`: Deterministic algebraic models and predictive analysis.
- `datasets/`: Real-world biological sequence fragments and hierarchical DAGs.
- `docs/`: Technical Reference (PDF), Phase Results (1-6), and Modeling Philosophy.
- `benchmarks/`: Multi-backend, scalability-aware benchmarking suite.

## Validation and Reproducibility
- **Cross-Platform Integrity:** All measurements are derived from actual execution on macOS, Linux, and Windows.
- **Deterministic Correctness:** 100% bit-exact equivalence verified across all algorithms and backends.