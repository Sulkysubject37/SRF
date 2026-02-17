# SRF Cache Model & Locality Rationale

## Rationale
The core goal of Phase 3 is to investigate whether runtime can be improved within a **fixed memory budget** by optimizing for data locality and cache hierarchy effects. 

## Memory Hierarchy Effects
- **Cache-Aware Tiling:** By adjusting tile sizes to fit within specific cache tiers (L1/L2), we aim to reduce the overhead of reloading data from DRAM/L3, potentially decreasing the wall-clock penalty of recomputation.
- **Locality Proxies:** We use recomputation distance and working set size as proxies for the "tightness" of data dependencies and their likelihood of staying in high-speed cache.

## No Asymptotic Change
All Phase 3 optimizations are constant-factor improvements related to hardware-software co-design. They do **not** change the mathematical complexity of the algorithms.
