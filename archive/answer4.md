# answer4.md — Mini preview graphs in the rail

## What changed

Added `MiniGraphWidget` — a tiny sparkline (~170×28 px) embedded inside each left-rail item, right below the subsystem name. Like the Windows Task Manager performance sidebar: you see a small live graph for every subsystem at a glance, even while viewing a different tab.

## Files

| File | Action |
|------|--------|
| `src/ui/minigraph.h` | **new** — MiniGraphWidget class |
| `src/ui/minigraph.cpp` | **new** — paint impl (polyline + fill gradient, no grid/labels) |
| `src/ui/mainwindow.h` | added `MiniGraphWidget` forward decl + `minis_` hash |
| `src/ui/mainwindow.cpp` | rail items now carry label + mini graph; each `updateXxx()` feeds the mini |
| `CMakeLists.txt` | added `src/ui/minigraph.cpp` to the sysperf target |

## How it works

- **`MiniGraphWidget`** is a stripped-down `GraphWidget`: same ring-buffer design, same `addSample()` API, but paints only a colored polyline + subtle fill gradient. No grid, no labels, no secondary series. Size hint: 170×28.

- **`ensurePanel()`** now builds each rail item as a composite widget:
  ```
  ┌──────────────┐
  │ CPU  (bold)  │
  │ ▁▂▃▄▅▆▇█▇▆  │  ← MiniGraphWidget (blue sparkline)
  └──────────────┘
  ```
  Colors match each subsystem: CPU=blue, Memory=purple, Disk=green, Net=teal, GPU=red.

- **Feed path**: Each `updateXxx()` already pushes a sample to the panel's main graph. Now it also pushes the same value to `minis_["cpu"]`, `minis_["mem"]`, etc. Mini graphs update every ~1 Hz tick along with everything else.

- **Rail width** bumped from 150–190px to 200–240px to fit the sparklines comfortably.

## Colors

| Subsystem | Line / Fill | Hex |
|-----------|------------|-----|
| CPU | kMiniCpuLine/Fill | `#2e9ef0` |
| Memory | kMiniMemLine/Fill | `#9b59d0` |
| Disk | kMiniDiskLine/Fill | `#27ae60` |
| Network | kMiniNetLine/Fill | `#16a085` |
| GPU | kMiniGpuLine/Fill | `#e04f5f` |

## Notes

- Mini graphs auto-scale vertically (with 15% headroom) and right-align the ring buffer, same as the main graph.
- Dynamic subsystems (disks with `disk:sda`, nets with `net:eth0`, GPUs with `gpu:0`) create their rail mini graphs on first appearance via the same `ensurePanel()` path.
- AGENTS.md was created in the repo root with the git-ops rule as requested.
