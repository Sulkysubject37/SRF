# SRF: Structured Recomputation Framework

SRF is a C++17 framework demonstrating a deterministic execution model that reduces the memory footprint of memory-intensive algorithms. It achieves this by applying controlled recomputation of intermediate states, rather than storing them all in memory. The framework is designed to preserve the exact semantic output of the algorithms it implements and does not claim to provide asymptotic speedups.

## Supported Algorithms

The SRF model has been applied to the following algorithms, preserving exact output equivalence with their baseline, high-memory counterparts:

*   **SRF-Needleman–Wunsch:** Applies memory restructuring to the classic sequence alignment algorithm.
*   **SRF-HMM:** Applies memory restructuring to Hidden Markov Model evaluators (both Forward and Viterbi algorithms).
*   **SRF-Graph-DP:** Applies memory restructuring to dynamic programming on Directed Acyclic Graphs (DAGs).

## Build Instructions

A C++17 compliant compiler is required. The framework has been validated with recent versions of GCC and Clang.

To build all binaries, run the provided build script from the root directory:
```bash
bash benchmarks/build.sh
```
Executables will be placed in the `build/` directory.

## Usage Examples

All binaries log structural metrics to a CSV file specified by the `SRF_LOG_FILE` environment variable. If not set, logs are written to `results/csv/benchmark_log.csv`.

#### SRF-Needleman–Wunsch
```bash
# Usage: ./build/nw_blocked <seq_a_path> <seq_b_path> <block_width> <group_size> <scale_name>
SRF_LOG_FILE=results.csv ./build/nw_blocked datasets/sequences/processed/human_xs.txt datasets/sequences/processed/neand_xs.txt 20 1 XS
```
*   `block_width`: The granularity parameter for the recomputation tiling strategy.

#### SRF-HMM (Viterbi Example)
```bash
# Usage: ./build/viterbi_checkpoint <seq_path> <segment_length> <group_size> <scale_name>
SRF_LOG_FILE=results.csv ./build/viterbi_checkpoint datasets/sequences/processed/human_s.txt 20 1 S
```
*   `segment_length`: The granularity parameter for checkpointing segments.

#### SRF-Graph-DP
```bash
# Usage: ./build/graph_recompute <graph_path> <group_depth> <group_size> <scale_name>
SRF_LOG_FILE=results.csv ./build/graph_recompute datasets/graphs/processed/go_subset_s.txt 4 1 S
```
*   `group_depth`: The granularity parameter for grouping nodes for recomputation.

## Output & Metrics

SRF outputs structural metrics that provide insight into the trade-offs between memory and recomputation. These are structural indicators, not performance guarantees.

*   **`recompute_events`**: The number of times an intermediate state was recomputed instead of being read from memory.
*   **`R_mem`**: The ratio of memory accesses that would have occurred in a baseline implementation versus those that occurred in the SRF implementation.
*   **`R_rec`**: The ratio of compute events that occurred in the SRF implementation versus those that would have occurred in a baseline implementation.
*   **`regime_classification`**: An analytical classification of the workload into `MEMORY_BOUND`, `COMPUTE_BOUND`, or `BALANCED` based on the trade-off between memory savings and recomputation cost.

## Determinism & Validation

The structural metrics produced by SRF have been verified to be stable and reproducible under various conditions. This includes validation across different compilers (GCC, Clang) and compiler optimization levels (-O0, -O2).

For a formal report on these validation tests, please see [docs/CROSS_ENVIRONMENT_VALIDATION.md](docs/CROSS_ENVIRONMENT_VALIDATION.md).

## Limitations

*   SRF does not guarantee a speedup; its primary goal is memory reduction, which may come at the cost of runtime overhead from recomputation.
*   Workloads with extremely large state spaces may still become memory-bound, though at a much larger scale than baseline implementations.
*   SRF is an exact, deterministic framework, not a heuristic accelerator.

## Citation

If you use SRF in your research, please cite it as follows:

```bibtex
@misc{srf_framework_2026,
  author       = {Arshad, MD.},
  institution  = {Department of Computer Science, Jamia Millia Islamia},
  title        = {{SRF: Structured Recomputation Framework}},
  year         = {2026},
  publisher    = {GitHub},
  journal      = {GitHub repository},
  howpublished = {\url{https://github.com/Sulkysubject37/SRF}}
}
```

## v1.1 — Code Hygiene Maintenance
This release introduces no changes to the SRF algorithms or experimental results. It is a maintenance update to improve repository hygiene by removing internal development comments and build artifacts. The core documentation and algorithmic implementations are preserved. The manuscript directory has been excluded from the repository via .


## v1.1 — Code Hygiene Maintenance
This release introduces no changes to the SRF algorithms or experimental results. It is a maintenance update to improve repository hygiene by removing internal development comments and build artifacts. The core documentation and algorithmic implementations are preserved. The manuscript directory has been excluded from the repository via `.gitignore`.

