# SRF Technical Reference: Implementation and Architectural Evolution

This document provides a comprehensive technical reference for the Structured Recomputation Framework (SRF), detailing the algorithmic transformations from storage-dominated dynamic programming to self-stabilizing, locality-aware, and algebraically modeled deterministic recomputation.

SRF is an algorithmic memory restructuring framework, not a new algorithmic family. All transformations preserve mathematical correctness and biological validity.

\newpage

## Phase 1: Baseline Architecture

Phase 1 established reference implementations to define ground truth behaviour, ensuring correctness and reproducibility across platforms.

### 1.1 Implementation Mechanics

#### Needleman–Wunsch (Global Alignment)
**Data Structure**
```cpp
std::vector<std::vector<int>> dp(N+1, std::vector<int>(M+1));
```
**Recurrence Relation**
For sequences A and B:
$$dp[i][j] = \max \begin{cases} dp[i-1][j-1] + \text{match\_or\_mismatch}(A[i], B[j]) \\ dp[i-1][j] + \text{gap\_penalty} \\ dp[i][j-1] + \text{gap\_penalty} \end{cases}$$

**Properties**
* Full matrix materialization
* $O(N \times M)$ memory
* No recomputation
* Memory-bandwidth intensive

#### HMM Inference (Forward / Viterbi)
**Data Structure**
$T \times S$ matrix where $T$ = observation length and $S$ = hidden states.

**Forward Recurrence**
$$\alpha_t(j) = \sum_i \alpha_{t-1}(i) \cdot a_{ij} \cdot b_j(o_t)$$

**Viterbi Recurrence**
$$\delta_t(j) = \max_i \delta_{t-1}(i) \cdot a_{ij} \cdot b_j(o_t)$$

**Properties**
* Entire trellis stored
* $O(T \times S)$ memory
* Sequential dependency

### 1.2 Baseline Performance Metrics
Phase 1 established the performance floor across platforms: macOS (~1.1 MB floor), Linux (~3.4 MB), and Windows (~4.4 MB).

![Phase 1 Global Master Profile](/Users/sulky/Documents/SRF/docs/phase_1_global_master_profile.png)

\newpage

## Phase 2: Functional SRF (Memory Restructuring)

Phase 2 replaced stored intermediate state with deterministic recomputation. 

### 2.1 Needleman–Wunsch (Space-Reduced / Blocked)
* **The Alternating Buffer Strategy:** Replaced the full matrix with linear buffers `prev` and `curr`.
* **Memory Lifecycle:** Only row ($i-1$) is retained during row $i$ evaluation. 
* **Recomputation Mechanics:** For transient cells $(i, j)$, recomputation events are logged to quantify the arithmetic work required to recover evicted state.

![Phase 2 Global Master Profile](/Users/sulky/Documents/SRF/docs/phase_2_global_master.png)

\newpage

## Phase 3: Locality Optimization

Phase 3 improved speed without increasing the $O(N)$ footprint by aligning recomputation with the CPU cache.

### 3.1 Needleman–Wunsch: Cache-Aware Tiling
* **The Strategy:** Align blocks with the CPU's physical cache hierarchy.
* **Cache Budget Model:** $$\text{TileSize}^2 \times 4 \text{ bytes} \le \text{CacheBudget}$$
* **Impact:** Reduced DRAM traffic, allowing SRF-Optimized variants to outperform baseline implementations on Linux and Windows.

![Phase 3 Global Master Profile](/Users/sulky/Documents/SRF/docs/phase_3_global_master.png)

\newpage

## Phase 4: Architectural Abstraction

Phase 4 introduced a formal separation between algorithmic logic and primitive computational execution.

### 4.1 Backend Interface Layer (`IBackend`)
SRF became "backend-agnostic" via a common primitive API, supporting CPU and simulated GPU backends with bit-exact correctness.

![Phase 4 Global Master Profile](/Users/sulky/Documents/SRF/docs/phase_4_global_master.png)

\newpage

## Phase 5: Granularity & Analytical Models

Phase 5 targeted the amortization of recomputation management overhead.

### 5.1 Granularity Amortization
Validation confirmed that management overhead follows an **Inverse-Linear Scaling Law**:
$$\text{Management\_Events} \propto \frac{1}{G}$$
Coarsening granularity ($G > 1$) reduced recompute bookkeeping by up to **100x**.

### 5.2 Deterministic Explanatory Models
SRF implemented platform-agnostic cost scorecards:
$$\text{Cost Ratio} = \frac{\text{Recompute Events}}{\text{Total Compute} - \text{Recompute Events}}$$

![Phase 5 Global Master Profile](/Users/sulky/Documents/SRF/docs/phase_5_global_master.png)

\newpage

## Phase 6 & 7: Workloads & Extreme Scaling

Phases 6 and 7 characterized SRF against authentic biological datasets and pushed it to absolute hardware limits (N=1,000,000).

![Phase 7 Global Master Profile](/Users/sulky/Documents/SRF/docs/phase_7_global_master.png)

\newpage

## Phase 8: Regime Stability & Adaptive Policy

Phase 8 introduces self-stabilization into the SRF core through a deterministic observation and conservative adaptation layer.

### 8.1 Observation & Drift Detection
The framework tracks execution through normalized algorithmic ratios:
*   $R_{mem} = \text{memory\_access\_proxy} / \text{compute\_events}$
*   $R_{rec} = \text{recompute\_events} / \text{compute\_events}$

Verification confirms **100% bit-identical ratios** across macOS, Linux, and Windows, proving that SRF metrics capture fundamental algorithmic properties.

### 8.2 Conservative Adaptation Policy
When sustained drift is detected, the **Adaptation Policy** triggers parameter adjustments governed by Hysteresis and Cool-Down guards to maintain architectural stability.

![Phase 8 Global Stability Profile](/Users/sulky/Documents/SRF/docs/phase_8_global_master.png)

---

## Conclusion

SRF demonstrates that memory footprint reduction via deterministic recomputation is not only viable but highly efficient. By decoupling **Hardware** (Backends), **Locality** (Cache), **Granularity** (Management), and **Control** (Stability), the framework provides a robust foundation for scaling bioinformatics algorithms on memory-constrained heterogeneous systems.
