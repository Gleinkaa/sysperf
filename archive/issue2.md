erik@laptop-erik-linux:~/source/sysperf$ cd ~/source/sysperf
git pull

# Build (should reach 100% now — main binary compiles)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j8

# Run tests (all 7 should pass)
ctest --test-dir build

# Launch the app
./scripts/start.sh
Already up to date.
-- Could NOT find XKB (missing: XKB_LIBRARY XKB_INCLUDE_DIR) (Required is at least version "0.5.0")
-- Configuring done (0.2s)
-- Generating done (0.0s)
-- Build files have been written to: /home/erik/source/sysperf/build
[  1%] Building CXX object CMakeFiles/sysperf_core.dir/src/collector/gpu_fdinfo.cpp.o
[  3%] Linking CXX static library libsysperf_core.a
[ 16%] Built target sysperf_core
[ 16%] Built target test_budget_smoke_autogen_timestamp_deps
[ 16%] Built target test_fdinfo_autogen_timestamp_deps
[ 16%] Built target test_meminfo_autogen_timestamp_deps
[ 16%] Built target test_netdev_autogen_timestamp_deps
[ 16%] Built target test_self_autogen_timestamp_deps
[ 16%] Built target sysperf_autogen_timestamp_deps
[ 16%] Built target test_cpu_autogen_timestamp_deps
[ 16%] Built target test_diskstats_autogen_timestamp_deps
[ 25%] Built target test_meminfo_autogen
[ 25%] Built target test_diskstats_autogen
[ 25%] Built target test_self_autogen
[ 25%] Built target test_budget_smoke_autogen
[ 25%] Built target test_netdev_autogen
[ 26%] Built target test_fdinfo_autogen
[ 28%] Built target sysperf_autogen
[ 37%] Built target test_cpu_autogen
[ 37%] Linking CXX executable test_netdev
[ 37%] Linking CXX executable test_diskstats
[ 37%] Linking CXX executable test_self
[ 37%] Linking CXX executable test_meminfo
[ 39%] Linking CXX executable test_budget_smoke
[ 41%] Linking CXX executable test_fdinfo
[ 42%] Linking CXX executable test_cpu
[ 44%] Building CXX object CMakeFiles/sysperf.dir/src/main.cpp.o
[ 71%] Built target test_diskstats
[ 71%] Built target test_self
[ 71%] Built target test_meminfo
[ 71%] Built target test_netdev
[ 71%] Built target test_budget_smoke
[ 73%] Building CXX object CMakeFiles/sysperf.dir/src/ui/mainwindow.cpp.o
[ 78%] Built target test_fdinfo
[ 83%] Built target test_cpu
[ 85%] Linking CXX executable sysperf
[100%] Built target sysperf
Internal ctest changing into directory: /home/erik/source/sysperf/build
Test project /home/erik/source/sysperf/build
    Start 1: test_cpu
1/7 Test #1: test_cpu .........................   Passed    0.00 sec
    Start 2: test_meminfo
2/7 Test #2: test_meminfo .....................   Passed    0.00 sec
    Start 3: test_diskstats
3/7 Test #3: test_diskstats ...................   Passed    0.00 sec
    Start 4: test_netdev
4/7 Test #4: test_netdev ......................   Passed    0.00 sec
    Start 5: test_fdinfo
5/7 Test #5: test_fdinfo ......................   Passed    0.00 sec
    Start 6: test_self
6/7 Test #6: test_self ........................   Passed    0.00 sec
    Start 7: test_budget_smoke
7/7 Test #7: test_budget_smoke ................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 7

Total Test time (real) =   0.02 sec
--- preflight ---
=== preflight: sysperf (desktop GUI) ===
  [OK]   version.txt = 0.1.0
  [OK]   STATUS = active
  [OK]   DEPLOYMENT.md present
  [OK]   README.md present
  [OK]   CLAUDE.md present (48 lines)
  [FAIL] missing required env vars: SYSPERF_CONFIG
  [OK]   config/default.json is valid JSON
  [WARN] release.json missing (generated at start)
  [OK]   no banned literals
--- desktop GUI checks ---
  [OK]   cmake: cmake version 3.28.3
  [OK]   g++ present
  [OK]   CMakeLists.txt present
  [OK]   src/main.cpp present
  [OK]   tests/ present
  [OK]   NVML/libdrm not hard-linked (dlopen only)
  [OK]   no privilege-escalation machinery (single unprivileged process)

