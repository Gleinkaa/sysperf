# sysperf — a lightweight Linux system-performance monitor

`sysperf` brings the **information density of the Windows Task Manager *Performance* tab** to
the Linux desktop — CPU, Memory, Disk, Network and GPU, each with a rolling 60-second graph and
aggregate stat readouts — while staying small enough that **it does not consume the resources it
is meant to measure** (PSS target near `btop`, ~20–40 MB; soft alarm at 50 MB / 1% of one core).

> **Scope:** the *Performance tab only.* No process table, no "End task", no privilege
> escalation. The app is a **single unprivileged process** — no root, no capabilities, no
> polkit, no D-Bus helper. Everything it shows is readable as your own user.

It is **not** a pixel clone of Windows. It reproduces Task Manager's *information architecture*
in a native Qt idiom, using **honest Linux semantics** (e.g. "In use" = `MemTotal − MemAvailable`,
and slab counters are labelled as slab — never faked as Windows "paged/non-paged pool").

## What it shows

| Panel | Source | Honest mapping |
|---|---|---|
| **CPU** total + per-core %, current GHz, model | `/proc/stat`, `/sys/.../scaling_cur_freq`, `/proc/cpuinfo` | util = busy-jiffy delta / total-jiffy delta |
| **Memory** In use / Available / Cached / Committed / Swap / Slab | `/proc/meminfo` | In use = `MemTotal − MemAvailable`; Cached = `Cached + Buffers + SReclaimable`; "pool" → `SReclaimable`/`SUnreclaim` slab |
| **Disk** %active + read/write throughput, SSD/HDD | `/proc/diskstats`, `/sys/block/*/queue/rotational` | %util = `io_ticks` delta / measured dt |
| **Network** send/receive throughput, link speed | `/proc/net/dev`, `/sys/class/net/*/speed` | bytes delta / measured dt |
| **GPU** util, VRAM, temp, clock, power | NVML (NVIDIA) + DRM `drm-engine-*` fdinfo (Intel/AMD) + sysfs/hwmon | dlopen'd, graceful degradation |

Generic across **NVIDIA / Intel / AMD** and any number of CPUs / disks / NICs / GPUs. If a
library or device is absent, that panel is simply omitted — the app never hard-links NVML/libdrm.

## Architecture (one sentence)

A **dedicated worker thread** ticks a `QTimer` at 1 Hz, reads `/proc` & `/sys`, builds an
**immutable `Snapshot`** and emits it **by value** over a queued signal; the **GUI thread owns
the ring buffers** and only paints — so the UI can never freeze even if a collector stalls.
Details in [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md).

## Build & run

Linux only. See [`DEPLOYMENT.md`](DEPLOYMENT.md) — the canonical runbook.

```bash
sudo apt install build-essential cmake qt6-base-dev libdrm-dev
./scripts/start.sh          # preflight → version → cmake build → run
```

Tests: `ctest --test-dir build --output-on-failure` (fixture-based collector tests + a budget
smoke test). Packaging: `.deb` + AppImage scaffolding under [`packaging/`](packaging/) (no
Snap/Flatpak — sandboxing would silently break the cross-process collectors).

## License

App code: MIT (see [`LICENSE`](LICENSE)). Qt 6 is used under **LGPLv3 via dynamic linking only**.
No GPL'd tool code (e.g. nvtop/btop) is copied; the fdinfo collector is an independent
implementation of the public `drm-engine-*` counter method.
