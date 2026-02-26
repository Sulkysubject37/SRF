# Cross-Environment Structural Validation

## 1. Scope of Validation

The purpose of this document is to record the structural invariance of the Structured Recomputation Framework (SRF) across a series of controlled, cross-environment tests. These tests were designed to validate the deterministic behavior and stability of core algorithmic metrics under different build and execution conditions.

This document and the tests it describes do **not** validate performance superiority. All timing data captured within the Continuous Integration (CI) environment is considered non-scientific and is not used for performance claims.

## 2. Compiler Diversity Validation

The SRF was compiled and executed using two distinct compilers: GCC and Clang.

Upon comparison of the outputs, all observed structural metrics were invariant across the tested compilers. This includes, but is not limited to, `R_mem`, `R_rec`, `recompute_events`, and all regime classifications. No semantic divergence in the framework's analytical output was observed between the two compiler environments.

## 3. Optimization-Level Validation

The SRF was compiled using GCC with two different optimization levels: `-O0` (no optimization) and `-O2` (a standard level of optimization).

The structural outputs of both builds were identical. While minor runtime differences were noted, these are expected and were excluded from the structural analysis. The results confirm that the application of standard compiler optimizations did not alter the structural or analytical output of the framework.

## 4. Determinism Re-Execution Validation

To verify deterministic behavior, the SRF was executed twice under identical conditions, producing two distinct log files. The resulting logs were compared for bit-exact equivalence after excluding fields known to have expected variability.

The `runtime_us` and `peak_memory_kb` fields were excluded from the comparison. All remaining structural fields were found to be bit-exact across the two runs, confirming deterministic reproducibility for the following categories:

*   `recompute_events`
*   `R_mem`
*   `R_rec`
*   All regime classifications and analytical outputs

## 5. Memory-Constrained Execution Stability

The SRF was executed under an artificially imposed memory constraint of 256MB.

Under this constraint, the core C++ binaries executed successfully and produced structurally correct metrics. This test validates the execution stability of the SRF binaries under the tested memory pressure. This validation does not evaluate the behavior of the post-processing Python analysis scripts under the same constraint, nor does it claim universal operability under arbitrary memory limits.

## 6. Summary of Verified Structural Properties

The validation exercises described in this document have verified the following properties of the SRF under the specified test conditions:

✔ Structural metric invariance across tested compilers (GCC, Clang)
✔ Optimization-level structural invariance (`-O0`, `-O2`)
✔ Deterministic reproducibility of structural outputs
✔ Stable execution under the tested memory constraint (256MB)

## 7. Explicit Non-Claims

This validation does **not** claim:
*   Hardware independence
*   Performance equivalence across platforms
*   Universality across all possible compilers or compiler versions
*   Universality across all possible memory limits
*   Algorithmic superiority over any baseline implementation
