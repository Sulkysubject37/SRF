# Phase 5-B: Modeling Philosophy

## Explanatory over Predictive
The models introduced in Phase 5-B are **explanatory**, not predictive guarantees. Their primary objective is to provide an algebraic framework for understanding *why* SRF behaves as it does, rather than asserting exact execution times.

## Metric-Driven derivation
All analytical logic is derived from observable quantities logged in the CSV schema (e.g., `compute_events`, `working_set_proxy`). By relying on measured metrics rather than machine-specific constants, the models remain hardware-agnostic and scientifically auditable.

## No Hardcoded Constants
SRF forbids the use of fitted constants or empirical tuning. The models use direct algebraic relationships (e.g., $1/G$ scaling) to describe recomputation efficiency and cache-line interactions.

## Interpretation Limits
Model outputs (labels like "FIT" or "SPILL", ratios like "Cost Ratio") should be interpreted as relative indicators of architectural efficiency. They are not universal laws and may be influenced by platform-specific OS scheduling or hardware prefetchers that are not explicitly captured by the framework metrics.
