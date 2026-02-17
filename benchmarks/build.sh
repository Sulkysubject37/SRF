#!/usr/bin/env bash
set -e

echo "[SRF] Building C++ Baselines..."

# Output directory for binaries
mkdir -p build

# Compile baselines/dp
g++ -std=c++17 -O3 baselines/dp/needleman_wunsch.cpp -o build/needleman_wunsch
g++ -std=c++17 -O3 baselines/dp/viterbi.cpp -o build/viterbi
g++ -std=c++17 -O3 baselines/dp/forward.cpp -o build/forward

# Compile baselines/graph
g++ -std=c++17 -O3 baselines/graph/graph_dp.cpp -o build/graph_dp

echo "[SRF] Build completed."
