#!/usr/bin/env bash
set -e

echo "[SRF] CI Execution Starting..."

# Ensure we are in the root directory
cd "$(dirname "$0")/.."

echo "[SRF] Running Tests..."
bash tests/run_tests.sh

echo "[SRF] Running Benchmarks..."
bash benchmarks/run_benchmarks.sh

echo "[SRF] CI Execution Completed Successfully."