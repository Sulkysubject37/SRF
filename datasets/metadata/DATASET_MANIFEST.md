# SRF Phase 6 Dataset Manifest

## 1. Sequence Datasets (NW / HMM)
**Source:** NCBI RefSeq
- **Sequence A:** Human Mitochondrial Genome (NC_012920.1)
- **Sequence B:** Neanderthal Mitochondrial Genome (NC_011137.1)
- **Status:** Real-world evolutionary variants.
- **Processing:** Fixed-length head fragments (XS to XL scaling ladder).

## 2. Graph Datasets (Graph-DP)
**Source:** Gene Ontology (GO) Consortium
- **File:** `go-basic.obo`
- **Structure:** Directed Acyclic Graph (DAG) derived from `is_a` hierarchical relationships.
- **Processing:** Deterministic node-subset selection (first N terms) to preserve top-level hierarchy.

## 3. Scaling Ladder Definitions
| Scale | Seq Length (NW/HMM) | Node Count (Graph-DP) |
| :--- | :--- | :--- |
| XS | 200 | 500 |
| S | 500 | 1,000 |
| M | 1,000 | 2,000 |
| L | 2,000 | 5,000 |
| XL | 4,000 | 10,000 |

## 4. Reproducibility
All datasets are processed via `scripts/prepare_sequences.py` and `scripts/prepare_graphs.py` using fixed extraction rules and no stochastic sampling.
