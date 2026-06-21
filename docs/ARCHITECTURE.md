# Architecture

## Threads

```
+--------------------------+        queued signal (Qt::QueuedConnection)        +-----------------------+
| CollectorWorker (QObject)|  -- emit snapshotReady(Snapshot BY VALUE) -->       | GUI thread (MainWindow)|
|  moved to a QThread      |                                                     | - owns ring buffers    |
|  - QTimer @ interval_ms  |                                                     |   (in each GraphWidget) |
|    created INSIDE the     |                                                     | - paints only; never    |
|    thread (start() slot)  |                                                     |   blocks on collectors  |
|  - reads /proc, /sys      |                                                     | - per-panel stat labels |
|  - NVML (dlopen) + fdinfo |                                                     +-----------------------+
+--------------------------+
```

The GUI thread only paints. Even if a collector stalls (e.g. the per-process
`fdinfo` walk on a busy box), the window keeps repainting the last data — the
worker is a separate thread.

## Load-bearing threading invariants

These are the "the worker thread is a lie if any is wrong" rules from the plan.
Enforced in `src/collector/worker.cpp` and `src/main.cpp`:

1. **The `QTimer` is created inside the worker thread.** `CollectorWorker`'s
   constructor runs on the GUI thread (the object is created there before
   `moveToThread`). Creating the timer there would bind it to the GUI event loop.
   Instead it is created in `start()`, a slot connected to `QThread::started`, which
   runs after the move — so the timer lives on, and fires on, the worker thread.
2. **`qRegisterMetaType<Snapshot>()` is called before the first emit** (in
   `main()`). Without it, a queued connection silently drops the custom struct.
3. **`Snapshot` crosses the boundary by value** — an immutable copy. No shared
   pointers to mutable state; no locks needed.
4. **The worker emits only the latest sample.** The rolling 60 s history lives on
   the GUI side: each `GraphWidget` owns its ring buffer and appends on each
   snapshot. The cross-thread payload stays tiny (a few scalars + small vectors).
5. **Rates use measured `CLOCK_MONOTONIC` dt**, not the nominal 1 s. The timer
   drifts and the thread is scheduled late, so disk `%util`, throughput, and self
   CPU divide by the real elapsed time. (Total CPU% uses the jiffy ratio, which is
   intrinsically normalised by the shared interval and needs no explicit dt.)

## Collectors (`src/collector/`, pure C++ — no Qt)

Each takes a **procfs/sysfs root as a parameter**, so fixtures can stand in for the
live tree (see `tests/fixtures/`). Each exposes static parse/derive functions
(unit-tested) plus a stateful `update()` that diffs against the previous sample.

| File | Source | Output |
|---|---|---|
| `cpu` | `/proc/stat`, `/proc/cpuinfo`, `/sys/.../scaling_cur_freq` | total + per-core %, GHz, model |
| `meminfo` | `/proc/meminfo` | honest In use / Available / Cached / Committed / Swap / Slab |
| `diskstats` | `/proc/diskstats`, `/sys/block/*/queue/rotational` | %active, read/write B/s, SSD/HDD |
| `netdev` | `/proc/net/dev`, `/sys/class/net/*/speed` | rx/tx B/s, link speed |
| `gpu_nvml` | NVML (`dlopen` `libnvidia-ml.so.1`) | NVIDIA util/VRAM/temp/clock/power |
| `gpu_fdinfo` | `/proc/*/fdinfo/*` `drm-engine-*`, hwmon | Intel/AMD util + temp |
| `self_metrics` | `/proc/self/smaps_rollup`, `/proc/self/stat` | own PSS + CPU% (budget guardrail) |

### Honest Windows→Linux mapping (the traps)

- **In use = `MemTotal − MemAvailable`** (not `− MemFree`; MemFree ignores
  reclaimable cache and lies once cache fills).
- **Cached = `Cached + Buffers + SReclaimable`**.
- **"Paged / non-paged pool" → `SReclaimable` / `SUnreclaim` slab**, relabelled
  honestly. Linux has no such pools; slab is the closest analog.

### Graceful degradation (non-negotiable)

NVML and libdrm are **never hard-linked**. NVML is `dlopen`'d at runtime; if it or
the driver is absent the NVIDIA panel is simply omitted. `fdinfo` is plain file
parsing (no libdrm calls). Any absent library/device → that panel disappears,
the app still starts. This is what makes one binary work across NVIDIA / Intel /
AMD and any count of CPUs / disks / NICs / GPUs.

## Rendering (`src/ui/`)

- `GraphWidget` — a `QWidget` whose `paintEvent` draws a `QPainterPath` polyline +
  gradient fill from its ring buffer. **No QtCharts, no `QOpenGLWidget`, no FPS
  loop.** `update()` is called only when a new ~1 Hz sample arrives, and only while
  visible; the ring buffer keeps filling while hidden so history is intact when the
  tab is shown.
- **HiDPI:** the grid is drawn with a width-0 (cosmetic) pen at `round(x)+0.5` so
  lines stay 1 device pixel and crisp under fractional scaling; only the data line
  is antialiased.
- `CoreBars` — a compact per-logical-CPU utilisation grid (Task Manager's "logical
  processors" view) drawn in one `paintEvent`.
- `MainWindow` — a left rail + `QStackedWidget`; panels for CPU, Memory, and one
  per disk / NIC / GPU are created lazily as devices are discovered in snapshots.

## Budget guardrail

`self_metrics` reports the app's own PSS (from `smaps_rollup` — honest; RSS
double-counts shared Qt libs) and CPU% each tick. Over the soft limits
(PSS > 50 MB or CPU > 1% of one core, configurable) the status-bar readout turns
red and a rate-limited warning is logged. `tests/test_budget_smoke.cpp` exercises
the alarm; the plan promotes this to a CI pass/fail gate at the first full
prototype running all five collectors together.
