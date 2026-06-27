# Issue tracker — sysperf

## Build status (2026-06-21, Ubuntu + Qt6, erik@laptop-erik-linux)

### ✅ Compiles clean
- `sysperf_core` (static lib) — all 8 collector `.cpp` files
- `test_self`, `test_diskstats`, `test_fdinfo`, `test_cpu`, `test_netdev`, `test_budget_smoke`, `test_meminfo` — all 7 test executables compiled + linked

### ❌ Fixed: two compilation bugs in main binary

#### Bug 1 — `src/config.h:8`: `class QStringList;` illegal in Qt6
**Root cause:** In Qt6, `QStringList` is a `using` alias (`using QStringList = QList<QString>`) not a class. Forward-declaring it with `class` is a language error.

**Fix:** Replaced `class QStringList;` with `#include <QStringList>`.

**Affected:** `config.cpp`, `worker.cpp`, `main.cpp`, `mainwindow.cpp`, `mocs_compilation.cpp` (all files transitively including `config.h` before any Qt6 header).

---

#### Bug 2 — `src/ui/panel.h:14-15`: Qt forward declarations inside `namespace sysperf`
**Root cause:** `class QGridLayout;` and `class QLabel;` were declared inside `namespace sysperf {}`, which created `sysperf::QGridLayout` and `sysperf::QLabel` — separate incomplete types from the real `::QGridLayout` and `::QLabel` in Qt’s global namespace. Every use of `QLabel*`, `new QLabel(...)`, etc. inside the namespace resolved to the wrong (incomplete) type.

**Fix:** Replaced the two forward declarations with `#include <QGridLayout>` and `#include <QLabel>` (outside the namespace block). This provides the complete types from Qt.

**Affected:** `panel.cpp`, `mainwindow.cpp` (transitively via `#include "ui/panel.h"`).

---

### Verification (on Linux/WSL)
```bash
git pull
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j8          # should reach 100%
ctest --test-dir build           # should pass all 7 tests
```

### Notes
- `Could NOT find XKB` in CMake is harmless — Qt keyboard extension, not needed.
- Tests use a tiny fixture-based harness (`tests/test_util.h`), no GoogleTest.

---

### Raw build log (original, 2026-06-21)
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
... (compiled core lib + all 7 tests OK, main binary failed — see git history) ...
```
