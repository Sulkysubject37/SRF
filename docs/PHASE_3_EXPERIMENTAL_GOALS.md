# Phase 3 Experimental Goals (Fixed-Memory Runtime Study)

## Primary Goal
Investigate the **fixed-memory runtime behavior** of SRF variants. Can runtime decrease WITHOUT increasing the memory footprint?

## Measurement Philosophy
- **Deterministic Scheduling:** All node ordering and checkpoint layouts are deterministic, ensuring consistent behavior across runs.
- **Proxy Metrics:** We use `working_set_proxy` and `locality_proxy` to estimate cache effects since direct hardware counter measurement is restricted.
- **Fixed Memory Constraint:** All SRF variants in Phase 3 must maintain a peak memory footprint no larger than their Phase 2 counterparts.

## Limitations & Assumptions
- Hardware-specific behaviors (L1/L2 sizes) are not directly measured but are modeled through parameter sweeps.
- The study assumes a single-threaded execution model as per the SRF mandates.
