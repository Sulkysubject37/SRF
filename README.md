# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 2: SRF Implementation & Parametric Evaluation — COMPLETED**
- SRF variants (Blocked, Checkpointed, Recompute-Driven) for all 4 core algorithms are implemented.
- **Parametric Characterization:** Evaluated Runtime vs. Memory trade-offs across macOS, Linux, and Windows.
- **Verification:** 100% bitwise equivalence between SRF variants and baselines confirmed.
- Detailed results are available in [docs/PHASE_2_RESULTS.md](docs/PHASE_2_RESULTS.md).

## Repository Structure
- `baselines/`: Reference implementations of core algorithms.
- `srf/`: Structured recomputation implementations (O(N) space variants).
- `docs/`: Phase 1 & 2 Results, SRF Modeling, and Recomputation Strategies.
- `benchmarks/`: Parametric benchmarking and plot generation suite.
- `tests/`: Correctness verification (Equivalence testing).
- `results/`: Raw CI artifacts (git-ignored).

## Validation and Reproducibility
- **CI-Based Validation:** Automated testing on macOS, Linux, and Windows for every push.
- **Venv Management:** Python 3.10 environment (`will`) used for data analysis and visualization.
