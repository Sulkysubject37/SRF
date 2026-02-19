# SRF Model Definitions

## 1. Recomputation Cost Model
**Algebraic Form:**
$$	ext{Cost Ratio} = \frac{	ext{Recompute Events}}{	ext{Total Compute} - 	ext{Recompute Events}}$$
**Assumption:** Every recomputation event has a constant arithmetic cost relative to a base compute event.
**Interpretation:** High ratios indicate a compute-heavy recomputation penalty.

## 2. Cache-Fit Threshold Model
**Algebraic Form:**
$$	ext{Regime} = \begin{cases} 	ext{FIT} & 	ext{if } 	ext{WorkingSet} \le 	ext{CacheBudget} \ 	ext{SPILL} & 	ext{if } 	ext{WorkingSet} > 	ext{CacheBudget} \end{cases}$$
**Assumption:** The `working_set_proxy` accurately captures the active data footprint of the recomputation unit.

## 3. Accelerator Feasibility Model
**Logic:**
If $	ext{Granularity} \ge 32$, dispatch overhead is assumed to be amortized.
**Interpretation:** Qualitative indicator of whether a task is "coarse" enough for efficient GPU acceleration.

## 4. Working-Set Locality Model
**Algebraic Form:**
$$	ext{Locality Index} = \frac{	ext{Unit Reuse Proxy}}{	ext{Granularity Unit Size}}$$
**Interpretation:** Higher indices represent better temporal and spatial locality within the recomputation unit.
