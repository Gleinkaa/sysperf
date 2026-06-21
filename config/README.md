# Config sources, in priority order

`sysperf` is a desktop GUI, not a service — there is **no port and no `/api/version`**. It still
keeps a single, documented config so behaviour isn't scattered through the code. Keys resolve in
this order (later wins):

1. **compiled defaults** — baked into the binary; identical to `config/default.json`.
2. **`config/default.json`** — loaded at startup if found next to the binary, in the current
   working directory, or via `$SYSPERF_CONFIG`. No secrets.
3. **`.env`** — machine-local overrides (see `.env.example`). Optional.
4. **CLI flags** — `--proc-root`, `--sys-root`, `--interval-ms`, `--config`. Highest priority.

If a config file is malformed the app logs a warning and falls back to compiled defaults — it
never refuses to start over a cosmetic setting.

## Key reference

| Key | Source | Purpose |
|---|---|---|
| `collector.interval_ms` | default.json / `SYSPERF_INTERVAL_MS` / `--interval-ms` | Sample period (1000 = 1 Hz). |
| `collector.history_seconds` | default.json | Ring-buffer length per graph (seconds). |
| `collector.proc_root` | default.json / `SYSPERF_PROC_ROOT` / `--proc-root` | Procfs root the collectors read. Point at a fixture tree to debug. |
| `collector.sys_root` | default.json / `SYSPERF_SYS_ROOT` / `--sys-root` | Sysfs root the collectors read. |
| `budget.pss_mb_warn` | default.json | Soft alarm: warn if own PSS exceeds this (MB). |
| `budget.cpu_pct_warn` | default.json | Soft alarm: warn if own CPU exceeds this (% of one core). |
| `ui.theme_hint` | default.json | `system` lets the Qt platform theme decide (qt6-gtk-platformtheme). |
| `ui.start_panel` | default.json | Which rail entry is selected on launch. |
| `logging.level` | default.json | debug / info / warn / error (stderr). |

## Adding a new config key

1. Add the default to `config/default.json` **and** to the compiled defaults in
   `src/config.cpp` (keep them in sync — preflight greps for divergence is future work).
2. Document it in the table above.
3. If it can be overridden by env, add it to `.env.example`.
4. Read it via the central `Config` loader — never scatter `getenv(...)` through collectors.
