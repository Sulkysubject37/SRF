# Scaling Regimes & Cache Effects

## Input Scaling
- **Small (L1 Cache):** At very small input sizes, recomputation and baseline runtime are likely dominated by OS scheduling and function call overhead.
- **Medium (L2/L3 Cache):** This is the primary scaling regime of the SRF Phase 3 study. Performance depends on the alignment of tiles/checkpoints with cache capacities.
- **Large (DRAM):** As inputs exceed cache capacity, the memory-time trade-off is dominated by DRAM latency and page faulting.

## Platform Variability
- **Darwin (macOS):** Typically shows very lean memory floors, making it ideal for cache-budget studies at smaller scales.
- **Linux/Windows:** Higher base memory floors may mask cache effects for small-to-medium inputs.
