#!/usr/bin/env bash
set -e

echo "[SRF] Building C++ Baselines and SRF Variants (Phase 4 Backends)..."

# Output directory for binaries
mkdir -p build

# Common objects
g++ -std=c++17 -O3 -c srf/runtime/backend_selector.cpp -o build/backend_selector.o

# Compile baselines
g++ -std=c++17 -O3 baselines/dp/needleman_wunsch.cpp -o build/needleman_wunsch
g++ -std=c++17 -O3 baselines/dp/viterbi.cpp -o build/viterbi
g++ -std=c++17 -O3 baselines/dp/forward.cpp -o build/forward
g++ -std=c++17 -O3 baselines/graph/graph_dp.cpp -o build/graph_dp

# Compile SRF Variants with Backend Support
g++ -std=c++17 -O3 srf/dp/nw_blocked.cpp build/backend_selector.o -o build/nw_blocked
g++ -std=c++17 -O3 srf/dp/viterbi_checkpoint.cpp build/backend_selector.o -o build/viterbi_checkpoint
g++ -std=c++17 -O3 srf/dp/forward_checkpoint.cpp build/backend_selector.o -o build/forward_checkpoint
g++ -std=c++17 -O3 srf/graph/graph_recompute.cpp build/backend_selector.o -o build/graph_recompute

echo "[SRF] Build completed."
