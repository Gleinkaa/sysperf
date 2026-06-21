# Status: active

**Set:** 2026-06-21
**Reason:** Initial scaffold + full first implementation of the Performance-tab monitor.

---

Valid states: `active` | `frozen` | `archived`

- **active** — under development. Conforms to the standard. Runs cleanly.
- **frozen** — code preserved for reference; not developed. Use `STATUS_FROZEN.md` filename instead and explain in the body what the canonical replacement is.
- **archived** — moved to `B_projects/_archive/<YYYY-MM>/`. This file marks the snapshot date and last-known-good version.

## Build/test reality

This is a **Linux-only desktop app** (reads `/proc`, `/sys`, NVML, DRM fdinfo). It cannot
compile or run on Windows, which is where the source-of-truth Drive lives. The scaffold and
full source were written on Windows; **compilation + CTest must run on Ubuntu** (native or
WSL2). See `DEPLOYMENT.md`. As of the initial commit, no Ubuntu/WSL environment was available
on the authoring machine, so the code is **written but not yet compiled/tested on Linux**.
