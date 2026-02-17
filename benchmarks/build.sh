#!/usr/bin/env bash
set -e

echo "[SRF] Building C++ Baselines and SRF Variants..."

# Output directory for binaries
mkdir -p build

# Compile baselines
g++ -std=c++17 -O3 baselines/dp/needleman_wunsch.cpp -o build/needleman_wunsch
g++ -std=c++17 -O3 baselines/dp/viterbi.cpp -o build/viterbi
g++ -std=c++17 -O3 baselines/dp/forward.cpp -o build/forward
g++ -std=c++17 -O3 baselines/graph/graph_dp.cpp -o build/graph_dp

# Compile SRF Variants
g++ -std=c++17 -O3 srf/dp/nw_blocked.cpp -o build/nw_blocked
g++ -std=c++17 -O3 srf/dp/viterbi_checkpoint.cpp -o build/viterbi_checkpoint
g++ -std=c++17 -O3 srf/dp/forward_checkpoint.cpp -o build/forward_checkpoint
g++ -std=c++17 -O3 srf/graph/graph_recompute.cpp -o build/graph_recompute

echo "[SRF] Build completed."