#!/usr/bin/env bash
set -e

echo "CI execution starting..."

bash tests/run_tests.sh
bash benchmarks/run_benchmarks.sh

echo "CI execution completed."
