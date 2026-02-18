# GPU Backend Limitations & Caveats

## Transfer Overhead
Data transfer between Host (CPU) and Device (GPU) introduces latency. For small problem sizes, the `transfer_overhead_us` may dominate the total execution time, leading to performance degradation compared to the CPU backend.

## Device Memory Budget
The GPU backend respects an explicit `DeviceMemoryBudget_kb`. If the algorithmic working set exceeds this limit, the backend will signal a failure, and the runtime selector will fallback to the CPU.

## SIMD/Warp Prohibitions
In accordance with the SRF mandates, the GPU backend executes sequential primitives. It does **not** employ warp-level optimizations, shared memory tiling, or massive parallelization. It acts as a "remote compute engine" for the deterministic SRF schedule.

## Simulation Mode
In environments where CUDA or OpenCL toolchains are unavailable, the GPU backend operates in **Simulation Mode**, executing on the host while modeling accelerator-specific metrics (transfer latency and kernel launch counts).
