#!/usr/bin/env bash
# lock.sh — dependency "lock" for a C++/CMake project. There is no language-level
# lockfile here; dependencies are system packages (build-essential, cmake,
# qt6-base-dev, libdrm-dev) declared in DEPLOYMENT.md and packaging/debian/control.
# This script verifies the toolchain is present and that CMake can configure — the
# closest analog to "the lock is fresh."

set -euo pipefail
cd "$(dirname "$0")/.."   # project root

echo "→ checking build toolchain"
command -v cmake >/dev/null 2>&1 || { echo "cmake missing (apt install cmake)"; exit 1; }
command -v g++   >/dev/null 2>&1 || command -v clang++ >/dev/null 2>&1 \
  || { echo "no C++ compiler (apt install build-essential)"; exit 1; }

echo "→ dry-run CMake configure (verifies Qt6 + deps resolve)"
TMP_BUILD="$(mktemp -d)"
trap 'rm -rf "$TMP_BUILD"' EXIT
if cmake -S . -B "$TMP_BUILD" -DSYSPERF_BUILD_TESTS=ON >/dev/null 2>&1; then
  echo "lock.sh OK — toolchain + dependencies resolve."
else
  echo "lock.sh: CMake configure failed. Install: build-essential cmake qt6-base-dev libdrm-dev"
  exit 1
fi
