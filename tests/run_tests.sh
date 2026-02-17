#!/usr/bin/env bash
set -e

echo "[SRF] Starting Correctness Tests..."

# Ensure we are in the root directory
cd "$(dirname "$0")/.."

# Ensure binaries are built
bash benchmarks/build.sh

# Helper to run binaries with optional .exe
run_bin() {
    if [ -f "./build/$1.exe" ]; then
        ./build/$1.exe
    elif [ -f "./build/$1" ]; then
        ./build/$1
    else
        echo "[ERROR] Binary $1 not found in build/"
        exit 1
    fi
}

# Verifying non-empty output for all baselines
for alg in needleman_wunsch viterbi forward graph_dp; do
    echo "Checking $alg..."
    OUTPUT=$(run_bin $alg)
    if [ -z "$OUTPUT" ]; then
        echo "[FAIL] $alg produced no output"
        exit 1
    fi
    # Check if Algorithm: header is present
    if ! echo "$OUTPUT" | grep -q "Algorithm:"; then
        echo "[FAIL] $alg output is malformed"
        exit 1
    fi
    echo "[PASS] $alg"
done

echo "[SRF] All tests passed."
