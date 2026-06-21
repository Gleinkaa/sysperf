# Next session — hand-off instructions

## What was fixed (2026-06-21)

### Bug 1 (issue.md) — Qt6 compilation errors
Two header bugs prevented the main binary from compiling on Linux:
- `src/config.h`: `class QStringList;` → replaced with `#include <QStringList>` (Qt6 makes QStringList a `using` alias, not a class)
- `src/ui/panel.h`: `QLabel`/`QGridLayout` forward-declared inside `namespace sysperf` → replaced with `#include <QLabel>` / `#include <QGridLayout>` outside namespace

### Bug 2 (issue1.md + issue1.png) — onboard GPU VRAM shows always 0
- `src/collector/gpu_fdinfo.cpp`: never read VRAM for non-NVIDIA GPUs
- Added reading of `/sys/bus/pci/devices/<pdev>/mem_info_vram_{total,used}` (standard DRM sysfs)

## On the Linux machine (erik@laptop-erik-linux)

```bash
cd ~/source/sysperf
git pull

# Build (should reach 100% now — main binary compiles)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j8

# Run tests (all 7 should pass)
ctest --test-dir build

# Launch the app
./scripts/start.sh
```

## What to verify
1. **Build succeeds** — no more `qstringlist.h` or `sysperf::QLabel` errors
2. **Tests pass** — 7/7 CTest tests green
3. **GPU VRAM** — AMD dGPU should show used/total; Intel iGPU may still show "—" if kernel doesn't expose mem_info_vram_* (normal for Intel dynamic RAM)

## If something still broken
1. Paste the full error output into a new `issue2.md`
2. Include a screenshot as `issue2.png` if UI-related
3. git add + git commit + git push so this session can continue
