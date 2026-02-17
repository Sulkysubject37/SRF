# Structured Recomputation Framework (SRF)

SRF (Structured Recomputation Framework) is a research project focused on characterizing and implementing structured recomputation strategies for dynamic programming and graph-based algorithms.

## Project Scope
This project is currently in **Phase 1: Baseline Characterization**. The primary goal of this phase is to establish a rigorous set of reference implementations and measure their performance in a standard execution environment.

### Performance Claims
At this stage, SRF does not make any claims regarding performance improvements or optimizations. All implementations in the `baselines/` directory are designed for correctness and clarity to serve as a ground truth for future comparisons.

## Repository Structure
- `baselines/`: Reference implementations of core algorithms (Needleman-Wunsch, Viterbi, etc.).
- `srf/`: Planned implementations of structured recomputation variants.
- `docs/`: Experimental protocols, assumptions, and research documentation.
- `benchmarks/`: Scripts for building and executing performance measurements.
- `tests/`: Correctness verification suite.
- `results/`: Raw data and visualization outputs.

## Validation and Reproducibility
- **CI-Based Validation:** All code is automatically tested and benchmarked via GitHub Actions to ensure correctness and consistent measurement.
- **Cross-Platform Intent:** The framework is designed with cross-platform compatibility in mind, using standard C++17 and portable shell scripts.
- **Experimental Protocol:** See `docs/EXPERIMENTAL_PROTOCOL.md` for details on how measurements are conducted.
