# SRF Extreme-Scale Dataset Manifest

## 1. Large-Scale Sequences
**Source:** Deterministic Synthetic Generator (`scripts/generate_extreme_datasets.py`)
- **Generation Rule:** Fixed-seed random choice from {A, C, G, T}.
- **Scales:** 10k, 50k, 100k, 500k, 1M.
- **Seeds:** Length $L$ for Sequence A, $L+1$ for Sequence B.

## 2. Large-Scale Graphs
**Source:** Topological DAG Generator (`scripts/generate_extreme_datasets.py`)
- **Generation Rule:** Mostly sequential edges ($u 	o u+1$) with 10% jump edges ($u 	o u+rand(2,10)$).
- **Scales:** 10k, 50k, 100k, 250k nodes.
- **Seeds:** Node count $N$.
- **Structural Guarantee:** Valid Directed Acyclic Graph (DAG) by construction (all edges $u 	o v$ satisfy $u < v$).

## 3. Metadata
Complete parameter listings available in:
- `datasets_extreme/metadata/sequence_scales.csv`
- `datasets_extreme/metadata/graph_scales.csv`
