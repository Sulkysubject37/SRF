# Phase 4: Backend Abstraction & Scalability

## Backend Abstraction Philosophy
Phase 4 introduces a formal separation between SRF algorithmic logic and primitive computational execution. By defining the `IBackend` interface, SRF remains "backend-agnostic," allowing the same recomputation schedules to run on varied hardware (CPU, GPU, FPGA) without semantic divergence.

## GPU Optionality & Fallback
The GPU backend is strictly optional. The system performs safe runtime detection (simulated in this phase) and falls back to the high-reliability CPU backend if:
- GPU hardware is absent.
- Device memory budget is exceeded.
- The execution environment is a non-accelerated CI runner.

## Determinism Constraints
Regardless of the selected backend, SRF guarantees bit-exact output equivalence. Primitives executed on the GPU backend follow the same numeric flow as the CPU counterpart, ensuring that biological validity is preserved across all execution environments.

## No Performance Claims
The GPU backend in Phase 4 is an experimental accelerator. Due to transfer overheads and the sequential nature of SRF primitives, it may not provide universal speedups for all problem sizes. This study focuses on **scalability and multi-backend portability.**
