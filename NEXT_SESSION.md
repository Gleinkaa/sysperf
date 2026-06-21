# Next session — hand-off instructions

> ## ✅ Good from our side (2026-06-21)
> All three reported issues are fixed, committed, and pushed (`master` @ `29f067a`).
> The build compiles 100% and all 7 CTests pass (confirmed on your Ubuntu run in issue2.md),
> and the `start.sh` preflight blocker is resolved.
>
> **The only thing left is to run it on your Linux box** — we can't launch the GUI from
> Windows. Please `git pull` and run `./scripts/start.sh`; the sysperf window should now
> open. If it does, issue2 is closed. See **What to verify** below.
> If anything is still broken, drop the output in a new `issue3.md` + push.

## What was fixed (2026-06-21)

### Bug 1 (issue.md) — Qt6 compilation errors
Two header bugs prevented the main binary from compiling on Linux:
- `src/config.h`: `class QStringList;` → replaced with `#include <QStringList>` (Qt6 makes QStringList a `using` alias, not a class)
- `src/ui/panel.h`: `QLabel`/`QGridLayout` forward-declared inside `namespace sysperf` → replaced with `#include <QLabel>` / `#include <QGridLayout>` outside namespace

### Bug 2 (issue1.md + issue1.png) — onboard GPU VRAM shows always 0
- `src/collector/gpu_fdinfo.cpp`: never read VRAM for non-NVIDIA GPUs
- Added reading of `/sys/bus/pci/devices/<pdev>/mem_info_vram_{total,used}` (standard DRM sysfs)

### Bug 3 (issue2.md) — `start.sh` aborts: "missing required env vars: SYSPERF_CONFIG"
**Symptom:** On Ubuntu the build reached 100% and all 7 CTests passed, but `./scripts/start.sh`
died in preflight with `[FAIL] missing required env vars: SYSPERF_CONFIG` — so the app built and
tested clean but never launched.

**Why:** preflight check #4 validates the `.env.example` schema. The generic template rule was
"a key with an empty example value is a *required secret you must fill in*", so it flagged
`SYSPERF_CONFIG=` (empty) as missing and exited 1 (`start.sh` runs preflight under `set -e`).
But sysperf needs **no** env vars — `.env.example`'s own header says every key is optional, and an
empty value means "use the built-in default". So the bouncer was blocking a perfectly good app.

**Fix:** `scripts/preflight.sh` check #4 — a key *declared* in `.env.example` is now always treated
as optional (empty value = "use built-in default", never "required secret"). Nothing else changed;
the check still confirms `.env.example` exists and is parseable. Verified locally: that line now
prints `[OK] all required .env.example keys satisfied`.

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
3. **`start.sh` launches the GUI** — preflight should now print
   `[OK] all required .env.example keys satisfied` and proceed past preflight into the
   build + launch (the **previous blocker** was the `SYSPERF_CONFIG` FAIL). This is the
   key thing issue2.md was waiting on — confirm the sysperf window actually opens.
4. **GPU VRAM** — AMD dGPU should show used/total; Intel iGPU may still show "—" if kernel doesn't expose mem_info_vram_* (normal for Intel dynamic RAM)

### Known non-blocking warnings in issue2.md (no action needed)
- `Could NOT find XKB (missing: XKB_LIBRARY ...)` — cosmetic CMake/Qt platform message; build still completes.
- `[WARN] release.json missing (generated at start)` — expected; `version.sh` generates it on launch.

## If something still broken
1. Paste the full error output into a new `issue2.md`
2. Include a screenshot as `issue2.png` if UI-related
3. git add + git commit + git push so this session can continue
