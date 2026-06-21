#!/usr/bin/env bash
# start.sh — THE canonical launch. No alternatives.
# Runs preflight → version → cmake build → run the GUI binary.
# Linux only (the app reads /proc, /sys, NVML, DRM fdinfo).

set -euo pipefail
cd "$(dirname "$0")/.."   # project root

echo "--- preflight ---"
bash scripts/preflight.sh

echo "--- versioning ---"
bash scripts/version.sh

echo "--- configure + build ---"
BUILD_DIR="${SYSPERF_BUILD_DIR:-build}"
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="${SYSPERF_BUILD_TYPE:-Release}"
cmake --build "$BUILD_DIR" -j"$(nproc 2>/dev/null || echo 2)"

echo "--- launch ---"
exec "$BUILD_DIR/sysperf" "$@"
