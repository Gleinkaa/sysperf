Well hello, my good agent. I have good news for you, the test ran threw (i put the logs at the bottom of this file)

And i have new instructions: I will always write you issue<N>.md, and you answer with answer<N>.md, so we can communicate via simple git syncing (Nik, your human user, needs to learn with git, so we do use basic git functions - commiting, pushing, pulling). So, from NOW ON; please do not make the git operations for nik, he has to type them by himself. You jsut do the implementation work, nothing more, nothing Less. Please put this paragraph somewhere in an AGENTS.md or memory, this needs th surrive threw sessions. 

And another good news, i have a task todo: we are in issue4.md and u put your answer in answer4.md. 
The programm looks very good. But i need something done with the tab selection on the right side: It works selecting RAM, CPU, GPU etc, but the win task manager has small preview graphs on the left side. SO even when i am in an RAM tab for example, i can still see roughly how much cpu i am using)



Now the script output from issue3.md, it worked as metnioned:

scripts/start.sh 
--- preflight ---
=== preflight: sysperf (desktop GUI) ===
  [OK]   version.txt = 0.1.0
  [OK]   STATUS = active
  [OK]   DEPLOYMENT.md present
  [OK]   README.md present
  [OK]   CLAUDE.md present (48 lines)
  [OK]   all required .env.example keys satisfied
  [OK]   config/default.json is valid JSON
  [OK]   release.json present
  [OK]   no banned literals
--- desktop GUI checks ---
  [OK]   cmake: cmake version 3.28.3
  [OK]   g++ present
  [OK]   CMakeLists.txt present
  [OK]   src/main.cpp present
  [OK]   tests/ present
  [OK]   NVML/libdrm not hard-linked (dlopen only)
  [OK]   no privilege-escalation machinery (single unprivileged process)
=== preflight OK ===
--- versioning ---
release.json written:
  {
    "version": "0.1.0",
    "git_sha": "2e6e57e",
    "git_dirty": false,
    "built_at": "2026-06-27T14:39:48Z",
    "cxx": "g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0",
    "qt_version": "6.4.2",
    "src_sha": "71a26fe51427"
  }
--- configure + build ---
-- Could NOT find XKB (missing: XKB_LIBRARY XKB_INCLUDE_DIR) (Required is at least version "0.5.0")
-- Configuring done (0.1s)
-- Generating done (0.0s)
-- Build files have been written to: /home/erik/source/sysperf/build
[ 16%] Built target sysperf_core
[ 16%] Built target sysperf_autogen_timestamp_deps
[ 16%] Built target test_netdev_autogen_timestamp_deps
[ 16%] Built target test_self_autogen_timestamp_deps
[ 16%] Built target test_diskstats_autogen_timestamp_deps
[ 16%] Built target test_meminfo_autogen_timestamp_deps
[ 16%] Built target test_cpu_autogen_timestamp_deps
[ 16%] Built target test_fdinfo_autogen_timestamp_deps
[ 16%] Built target test_budget_smoke_autogen_timestamp_deps
[ 28%] Built target test_cpu_autogen
[ 28%] Built target test_budget_smoke_autogen
[ 28%] Built target test_fdinfo_autogen
[ 28%] Built target test_self_autogen
[ 28%] Built target test_netdev_autogen
[ 28%] Built target test_diskstats_autogen
[ 28%] Built target test_meminfo_autogen
[ 30%] Built target sysperf_autogen
[ 80%] Built target test_diskstats
[ 80%] Built target test_meminfo
[ 80%] Built target test_budget_smoke
[ 80%] Built target test_cpu
[ 80%] Built target test_fdinfo
[ 80%] Built target test_self
[ 80%] Built target test_netdev
[ 82%] Building CXX object CMakeFiles/sysperf.dir/src/main.cpp.o
[ 83%] Building CXX object CMakeFiles/sysperf.dir/src/ui/mainwindow.cpp.o
[ 85%] Linking CXX executable sysperf
[100%] Built target sysperf
--- launch ---
sysperf: over soft budget — PSS=45.8 MB, CPU=2.98% of one core (limits: 50 MB / 1.00%)
sysperf: over soft budget — PSS=46.1 MB, CPU=4.00% of one core (limits: 50 MB / 1.00%)
sysperf: over soft budget — PSS=46.1 MB, CPU=3.00% of one core (limits: 50 MB / 1.00%)
sysperf: over soft budget — PSS=46.1 MB, CPU=4.00% of one core (limits: 50 MB / 1.00%)
