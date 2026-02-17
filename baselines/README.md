# Baselines

The `baselines/` directory contains reference implementations of algorithms used in the SRF project.

## Purpose
The primary purpose of these implementations is to establish a **ground truth for correctness** and a **performance baseline** for subsequent research.

## Design Philosophy
- **Correctness-First:** All code is written for clarity and deterministic behavior.
- **Minimal Optimization:** These implementations avoid SIMD, threading, or other advanced optimizations to provide a pure sequential baseline.
- **Standard C++:** All baselines are implemented in C++17 without external dependencies.
