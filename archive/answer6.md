# Answer 6 — Custom icon, better stdout, --no-gui

## Files changed

| File | Action | Purpose |
|------|--------|---------|
| `packaging/sysperf.svg` | rewritten | Funky neon icon with sparkline + animated bars + glow effects |
| `src/sysperf.qrc` | **new** | Qt resource file embedding the SVG |
| `src/cli_output.h` | **new** | Header for terminal text formatter |
| `src/cli_output.cpp` | **new** | Renders Snapshot as readable, spaced terminal output |
| `src/main.cpp` | modified | Icon loading, `--no-gui` flag, CLI mode |
| `CMakeLists.txt` | modified | Added `cli_output.cpp` + `sysperf.qrc` to build |

## What each commit does

### Commit 1 — Funky icon
- Redesigned `packaging/sysperf.svg`: dark pill background, neon cyan sparkline with glow filter, four colored animated-style rising bars (blue/purple/green/teal), grid lines, pulse dot, and a subtle "S" monogram
- Created `src/sysperf.qrc` to embed the SVG as a Qt resource (`:/app-icon`)
- Added `app.setWindowIcon(QIcon(":/app-icon"))` in `main.cpp`
- Updated `CMakeLists.txt` to include the .qrc file

### Commit 2 — Better stdout
- Created `src/cli_output.h` and `src/cli_output.cpp` with `sysperf::cli::render(Snapshot)`
- Output format: sections delimited by `══`/`──` separator lines, padded columns with labeled stats:

```
══ sysperf 0.1.0 ═══════════════════════════════════════════

── CPU ───────────────────────────────────────────────────
  Total:  35.2%         Speed:  2.40 GHz       Cores:  16 logical
  Busiest core:  52%
  Model:  AMD Ryzen 9 5950X

── Memory ────────────────────────────────────────────────
  Used:   5.21 GiB     /  15.62 GiB    (33.4%)
  Avail:  10.41 GiB      Cached:  2.13 GiB
  Swap:   0 B           /  2.00 GiB
  Committed:  8.14 GiB / 17.62 GiB

── Disk ──────────────────────────────────────────────────
  nvme0n1        5.2%     active  │  R 12.3 MB/s     │  W 4.5 MB/s  [SSD]

── Network ───────────────────────────────────────────────
  eth0        R 98.5 Mbps       │  S 12.3 Mbps       │  Link 1 Gbps

── GPU ───────────────────────────────────────────────────
  RTX 3080            Util:  45.2%     Mem:  4.21 GiB / 10.00 GiB
                      Temp:  72 °C     Clock: 1800 MHz  Power: 220 W

── Self ──────────────────────────────────────────────────
  PSS:  24.3 MiB       CPU:  0.15%
```

- Uses existing `fmt::` helpers (bytes, percent, rate, bitrate, etc.)
- Skips first sample (no deltas yet), prints nothing for empty subsystems
- `--no-gui` shows an initial header line with version + interval

### Commit 3 — --no-gui flag
- Added `--no-gui` to the early arg parser in `main.cpp`
- When `--no-gui` is set: creates a `QCoreApplication` (no GUI toolkit) instead of `QApplication`
- Worker thread + collector infrastructure stays identical — same `snapshotReady` signal
- In CLI mode, connects `snapshotReady` to a lambda that calls `cli::render()` and `printf`s to stdout
- In GUI mode, connects to `MainWindow::onSnapshot` as before
- Added `--no-gui` to `--help` output
- Clean shutdown: `stop()` → `quit()` → `wait()` → `delete` works identically in both modes

## Build note

This project targets Linux (needs `/proc`, `/sys`, `dlfcn.h`, `readlink`). Won't compile on Windows. Build on Linux with:

```
cmake -B build -DSYSPERF_BUILD_GUI=ON -DSYSPERF_BUILD_TESTS=ON
cmake --build build
```

Run CLI mode:
```
./build/sysperf --no-gui --interval-ms 2000
```
