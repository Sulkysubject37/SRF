# Phase 8B: Conservative Adaptation Policy

## Philosophy of Conservatism
The adaptation policy in SRF Phase 8B is designed to be **reactive and stable**, not aggressive or speculative. The primary objective is to maintain execution stability under varying workloads, with a strict priority on preserving deterministic outcomes.

## Stability Protection Mechanisms
To prevent oscillation and ensure reliability, the policy enforces four mandatory guards:

1.  **Hysteresis Guard:** Adaptation is only triggered if the observed drift in recomputation ratios ($R_{rec}$) is sustained and significantly different (typically >5%) from the state at the last adaptation event.
2.  **Cool-Down Window:** A fixed deterministic window (e.g., 50-100 steps) during which all further adaptation is disabled, allowing the system to stabilize before re-evaluating drift.
3.  **Single-Parameter Rule:** Only one structural parameter (e.g., `TileSize` OR `GranularityUnit`) is modified per adaptation event to ensure clear cause-effect attribution.
4.  **Reversion Guard:** The policy is inherently reversible. If an adaptation leads to an unfavorable increase in drift or performance floor, the system will revert to the previous known-stable parameter set.

## Interpretation of Signals
The policy interprets the `DRIFT_CANDIDATE` signal from the detector. It treats a high $R_{rec}$ as a signal to increase granularity (amortize overhead) and a low $R_{rec}$ as an opportunity to reduce the checkpoint interval (save memory) without significant penalty.

## Determinism
All adaptation decisions depend exclusively on deterministic counters and drift signals. This ensures that the execution path remains identical across all platforms for a given input, preserving the bit-exact correctness of the biological results.
