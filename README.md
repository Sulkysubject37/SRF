# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Status
**Phase 1: Baseline Characterization — COMPLETED**
- Reference implementations for Needleman-Wunsch, Viterbi, Forward, and Graph-DP are established.
- Cross-platform performance (Runtime, Memory, and Cache Diagnostics) has been fully measured on macOS, Linux, and Windows.
- Detailed results are available in [docs/PHASE_1_RESULTS.md](docs/PHASE_1_RESULTS.md).

## Repository Structure
- `baselines/`: Reference implementations of core algorithms.
- `srf/`: Reserved for future structured recomputation variants.
- `docs/`: Experimental protocols and Phase 1 Baseline Results.
- `benchmarks/`: Scripts for building and executing performance measurements.
- `tests/`: Correctness verification suite.
- `results/`: Raw CI data and visualization outputs (ignored by git, see docs/ for consolidated results).

## Validation and Reproducibility
- **CI-Based Validation:** Automated testing on every push for macOS, Linux, and Windows.
- **Cross-Platform Support:** Verified using Standard C++17 and platform-aware measurement utilities.