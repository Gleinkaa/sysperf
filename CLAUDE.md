# Claude orientation — sysperf

**Version:** see `version.txt`
**Status:** see `STATUS.md` (must be `active` to develop here)
**Kind:** Linux **desktop GUI** (C++17 + Qt6 Widgets + CMake). No server, no port, no web dashboard.
**Runbook:** [`DEPLOYMENT.md`](DEPLOYMENT.md) — THE only doc that explains how to build & run.

## Hard rules

- Conform to [`A_AI_infra/rules/B_PROJECTS_DEPLOYMENT_STANDARD.md`](../../A_AI_infra/rules/B_PROJECTS_DEPLOYMENT_STANDARD.md).
  This is a GUI app, so the web-only bits of the standard (port, `/api/version`, FE↔BE banner)
  do not apply; the mandatory *shape* (`version.txt`, `STATUS.md`, `DEPLOYMENT.md`, `CLAUDE.md`,
  `README.md`, `config/`, `scripts/`, `.gitignore`, `.env.example`) is kept.
- Conform to [`A_AI_infra/rules/SECRETS_AND_SSH.md`](../../A_AI_infra/rules/SECRETS_AND_SSH.md). No secrets committed.
- All version references derive from `version.txt` → CMake generates `version.h`; `scripts/version.sh`
  writes `release.json`. Never hand-edit a version string elsewhere.
- **Single unprivileged process.** Never add root, capabilities, polkit, or a D-Bus helper. Scope
  is the **Performance tab only** — no process table / kill / privilege escalation.
- **Never hard-link NVML or libdrm.** `dlopen` NVML at runtime; parse fdinfo as plain files.
  Absent library/device ⇒ omit that panel.
- **No hardware/data names from any reference machine** anywhere in code, tests, or docs. Generic
  across NVIDIA/Intel/AMD and any count of CPUs/disks/NICs/GPUs.
- Threading invariants are load-bearing — read `docs/ARCHITECTURE.md` before touching the worker:
  QTimer is created **inside** the worker thread; `Snapshot` is `qRegisterMetaType`'d and emitted
  **by value**; GUI owns the ring buffers; CPU%/rates use **measured `CLOCK_MONOTONIC` dt**.

## Build & run

```bash
./scripts/start.sh      # Linux/WSL only. preflight → version → cmake build → run binary
ctest --test-dir build  # fixture collector tests + budget smoke test
```

Windows is source-of-truth storage only; the app cannot compile/run there.

## Where things live

- Core (no Qt): `src/model/`, `src/collector/` — pure `/proc` parsers, fixture-testable.
- Qt app: `src/collector/worker.*` (QObject + QTimer), `src/ui/`, `src/main.cpp`.
- Config defaults: `config/default.json` (mirrors the app's compiled defaults).
- Tests + fixtures: `tests/`. Packaging: `packaging/debian/`, `packaging/appimage/`.

## Don't

- Don't add an "alternative" launcher. `scripts/start.sh` is the only one.
- Don't pull in QtCharts, QML/Quick, OpenGL, or an FPS animation loop. Graphs are custom
  `QWidget::paintEvent`, repainted only on a new 1 Hz sample.
- Don't commit `release.json`, `build/`, or AppImage/`.deb` artifacts.
