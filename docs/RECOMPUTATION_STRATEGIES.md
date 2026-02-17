# Recomputation Strategies

This document explains the specific recomputation strategies implemented in Phase 2 of the SRF project.

## Blocked DP Recomputation (Needleman-Wunsch)
In the blocked recomputation variant, the Dynamic Programming (DP) matrix is conceptually partitioned into fixed-size tiles. 
- **Storage:** Only the boundary states (e.g., specific rows and columns) required for correctness are stored.
- **Recomputation:** When a value within the interior of a tile is needed (e.g., during backtrace or evaluation), the entire tile is recomputed from its nearest stored boundaries.

## Checkpointed Inference (Forward / Viterbi)
Checkpointing is used to reduce the memory footprint of Hidden Markov Model (HMM) algorithms.
- **Checkpoints:** Stored every `K` steps along the observation sequence.
- **Recomputation:** Intermediate probabilities are recomputed from the nearest preceding checkpoint.
- **Parametric Control:** The `K` interval allows for a direct trade-off between checkpoint storage and recomputation overhead.

## Graph Recomputation (Graph-DP)
For Directed Acyclic Graphs (DAGs), recomputation is driven by the node dependency structure.
- **Frontier Storage:** Minimal state is stored at the current evaluation frontier.
- **Aggregate Recomputation:** Predecessor aggregates or ancestor states are recomputed as needed during the evaluation of a node's distance or dependencies.
