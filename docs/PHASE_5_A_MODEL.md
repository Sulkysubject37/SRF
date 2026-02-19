# Phase 5-A: Granularity-Aware Recomputation

## Granularity-Aware Recomputation Philosophy
Phase 5-A introduces the concept of "Granularity" as a managed property of the Structured Recomputation Framework (SRF). While Phase 3 focused on cache-line efficiency, Phase 5-A formalizes the atomic unit of recomputation. By grouping individual computational steps into larger, deterministic "units," the framework can study the amortization of overhead associated with recomputation management and state recovery.

## Atomic Unit Definitions
The framework defines algorithm-specific atomic units via the `GranularityPolicy`:

*   **Tiles (Needleman-Wunsch):** A 2D block of size $G 	imes G$. Recomputation decisions and instrumentation (recompute vs. reuse) are performed at the tile boundary rather than the cell level.
*   **Segments (HMM Forward/Viterbi):** A 1D sequence of length $G$. The trellis is processed in segments, with recomputation events tracked per segment.
*   **Groups (Graph-DP):** A logical grouping of nodes based on their topological index. Recomputation dependencies are resolved at the group level to minimize management overhead in complex DAGs.

## Determinism Constraints
Granularity policies are strictly deterministic. The `GranularityPolicy` calculates unit identifiers and boundaries using fixed mathematical mapping ($idx / G$). This ensures that recomputation units are consistent across all backends (CPU/GPU) and platforms (macOS/Linux/Windows), preserving the bit-exact output equivalence of the SRF variants.

## Instrumentation Model for Units
Phase 5-A introduces two high-fidelity metrics:
*   **Unit Recompute Events:** Incremented only when the algorithm transitions into a new granularity unit that requires state reconstruction.
*   **Unit Reuse Proxy:** Measures how many individual computational steps (cells/nodes) are successfully resolved within a "hot" granularity unit before it is evicted or recomputed.

These metrics allow for a precise calculation of the "Granularity Efficiency Ratio," characterizing the overhead amortization of the framework.
