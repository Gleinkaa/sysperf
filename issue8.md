There are compiler warnings





cmake --build build -j 8
-- Could NOT find XKB (missing: XKB_LIBRARY XKB_INCLUDE_DIR) (Required is at least version "0.5.0")
-- Configuring done (0.1s)
-- Generating done (0.0s)
-- Build files have been written to: /home/erik/source/sysperf/build
[ 15%] Built target sysperf_core
[ 15%] Built target test_fdinfo_autogen_timestamp_deps
[ 15%] Built target test_netdev_autogen_timestamp_deps
[ 15%] Built target test_meminfo_autogen_timestamp_deps
[ 15%] Built target test_cpu_autogen_timestamp_deps
[ 15%] Built target test_diskstats_autogen_timestamp_deps
[ 15%] Built target test_budget_smoke_autogen_timestamp_deps
[ 15%] Built target sysperf_autogen_timestamp_deps
[ 15%] Built target test_self_autogen_timestamp_deps
[ 22%] Built target test_diskstats_autogen
[ 22%] Built target test_cpu_autogen
[ 22%] Built target test_netdev_autogen
[ 22%] Built target test_fdinfo_autogen
[ 24%] Built target test_meminfo_autogen
[ 25%] Built target test_budget_smoke_autogen
[ 27%] Built target test_self_autogen
[ 29%] Automatic MOC for target sysperf
[ 68%] Built target test_diskstats
[ 68%] Built target test_netdev
[ 68%] Built target test_meminfo
[ 68%] Built target test_cpu
[ 68%] Built target test_budget_smoke
[ 70%] Built target test_fdinfo
[ 77%] Built target test_self
[ 77%] Built target sysperf_autogen
[ 82%] Building CXX object CMakeFiles/sysperf.dir/src/cli_output.cpp.o
[ 82%] Building CXX object CMakeFiles/sysperf.dir/src/main.cpp.o
[ 82%] Building CXX object CMakeFiles/sysperf.dir/src/ui/mainwindow.cpp.o
/home/erik/source/sysperf/src/cli_output.cpp:32:68: warning: multi-character character constant [-Wmultichar]
   32 |     for (int i = static_cast<int>(out.size()); i < 64; ++i) out += '═';
      |                                                                    ^~~
/home/erik/source/sysperf/src/cli_output.cpp:40:41: warning: multi-character character constant [-Wmultichar]
   40 |     for (int i = 6; i < 64; ++i) out += '─';
      |                                         ^~~
/home/erik/source/sysperf/src/cli_output.cpp:57:42: warning: multi-character character constant [-Wmultichar]
   57 |     for (int i = 10; i < 64; ++i) out += '─';
      |                                          ^~~
/home/erik/source/sysperf/src/cli_output.cpp:83:41: warning: multi-character character constant [-Wmultichar]
   83 |     for (int i = 8; i < 64; ++i) out += '─';
      |                                         ^~~
/home/erik/source/sysperf/src/cli_output.cpp:101:42: warning: multi-character character constant [-Wmultichar]
  101 |     for (int i = 11; i < 64; ++i) out += '─';
      |                                          ^~~
/home/erik/source/sysperf/src/cli_output.cpp:117:41: warning: multi-character character constant [-Wmultichar]
  117 |     for (int i = 7; i < 64; ++i) out += '─';
      |                                         ^~~
/home/erik/source/sysperf/src/cli_output.cpp:139:41: warning: multi-character character constant [-Wmultichar]
  139 |     for (int i = 8; i < 64; ++i) out += '─';
      |                                         ^~~
/home/erik/source/sysperf/src/cli_output.cpp: In function ‘std::string sysperf::cli::{anonymous}::renderHeader()’:
/home/erik/source/sysperf/src/cli_output.cpp:32:68: warning: overflow in conversion from ‘int’ to ‘char’ changes value from ‘14849424’ to ‘-112’ [-Woverflow]
   32 |     for (int i = static_cast<int>(out.size()); i < 64; ++i) out += '═';
      |                                                                    ^~~
/home/erik/source/sysperf/src/cli_output.cpp: In function ‘std::string sysperf::cli::{anonymous}::renderCpu(const sysperf::CpuInfo&)’:
/home/erik/source/sysperf/src/cli_output.cpp:40:41: warning: overflow in conversion from ‘int’ to ‘char’ changes value from ‘14849152’ to ‘-128’ [-Woverflow]
   40 |     for (int i = 6; i < 64; ++i) out += '─';
      |                                         ^~~
/home/erik/source/sysperf/src/cli_output.cpp: In function ‘std::string sysperf::cli::{anonymous}::renderMem(const sysperf::MemInfo&)’:
/home/erik/source/sysperf/src/cli_output.cpp:57:42: warning: overflow in conversion from ‘int’ to ‘char’ changes value from ‘14849152’ to ‘-128’ [-Woverflow]
   57 |     for (int i = 10; i < 64; ++i) out += '─';
      |                                          ^~~
/home/erik/source/sysperf/src/cli_output.cpp: In function ‘std::string sysperf::cli::{anonymous}::renderDisks(const std::vector<sysperf::DiskSample>&)’:
/home/erik/source/sysperf/src/cli_output.cpp:83:41: warning: overflow in conversion from ‘int’ to ‘char’ changes value from ‘14849152’ to ‘-128’ [-Woverflow]
   83 |     for (int i = 8; i < 64; ++i) out += '─';
      |                                         ^~~
/home/erik/source/sysperf/src/cli_output.cpp: In function ‘std::string sysperf::cli::{anonymous}::renderNets(const std::vector<sysperf::NetSample>&)’:
/home/erik/source/sysperf/src/cli_output.cpp:101:42: warning: overflow in conversion from ‘int’ to ‘char’ changes value from ‘14849152’ to ‘-128’ [-Woverflow]
  101 |     for (int i = 11; i < 64; ++i) out += '─';
      |                                          ^~~
/home/erik/source/sysperf/src/cli_output.cpp: In function ‘std::string sysperf::cli::{anonymous}::renderGpus(const std::vector<sysperf::GpuSample>&)’:
/home/erik/source/sysperf/src/cli_output.cpp:117:41: warning: overflow in conversion from ‘int’ to ‘char’ changes value from ‘14849152’ to ‘-128’ [-Woverflow]
  117 |     for (int i = 7; i < 64; ++i) out += '─';
      |                                         ^~~
/home/erik/source/sysperf/src/cli_output.cpp: In function ‘std::string sysperf::cli::{anonymous}::renderSelf(const sysperf::SelfMetrics&)’:
/home/erik/source/sysperf/src/cli_output.cpp:139:41: warning: overflow in conversion from ‘int’ to ‘char’ changes value from ‘14849152’ to ‘-128’ [-Woverflow]
  139 |     for (int i = 8; i < 64; ++i) out += '─';
      |                                         ^~~
[ 84%] Linking CXX executable sysperf
[100%] Built target sysperf

## Answer

### Root cause

The source file `src/cli_output.cpp` uses **Unicode box-drawing characters** as C++ single-quote character literals:

```cpp
for (int i = 8; i < 64; ++i) out += '─';
```

But `'─'` is not a valid `char` literal — it's a *multi-character character constant*, which C++ interprets as a single `int` whose value depends on the platform's endianness. Since `─` (U+2500) is a 3-byte UTF-8 sequence (`E2 94 80`), the compiler packs those bytes into an `int`, producing values like `14849152` — and then warns about the overflow when converting back to `char`.

### Fix

Define the Unicode characters as `constexpr` string_view constants and concatenate their data:

```cpp
constexpr std::string_view HLINE = u8"─";
constexpr std::string_view THICK_HLINE = u8"═";
```

Then replace every occurrence of `'─'` with `HLINE` and `'═'` with `THICK_HLINE`:

```cpp
for (int i = 8; i < 64; ++i) out += HLINE;
```

Also added `#include <string_view>` to the includes.

### Files changed

- `src/cli_output.cpp` — defined `HLINE` / `THICK_HLINE` constants, replaced all 7 char-literal occurrences, added `<string_view>` include.

Good from our side
