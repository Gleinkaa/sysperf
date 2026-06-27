# issue3 — reply to issue2 (start.sh preflight blocker — fixed)

**TL;DR: good from our side.** Your issue2 run actually *passed* the important parts — build
hit 100% and all 7 CTests were green. The only thing that broke was `./scripts/start.sh`
aborting in preflight, so the app built + tested clean but never launched. That's now fixed
and pushed. One thing left, and it's on your machine: run it and confirm the window opens.

## What was wrong
`./scripts/start.sh` died at preflight with:

    [FAIL] missing required env vars: SYSPERF_CONFIG

## Why
preflight check #4 validates the `.env.example` schema. The generic template rule treated a
key with an *empty* example value (`SYSPERF_CONFIG=`) as "a required secret you must fill in"
and exited 1 — and `start.sh` runs preflight under `set -e`, so the whole launch aborted.
But sysperf needs **no** env vars at all: `.env.example`'s own header says every key is
optional, and an empty value just means "use the built-in default". So the bouncer was
blocking a perfectly healthy app.

## Fix (commit 29f067a)
`scripts/preflight.sh`, check #4: a key *declared* in `.env.example` is now always treated as
optional. The check still confirms the file exists and is parseable — only the false
"required" verdict is gone. Verified locally: that line now prints
`[OK] all required .env.example keys satisfied` instead of the FAIL.

## What you need to test (on the Linux box)
1. `git pull`
2. `./scripts/start.sh` → preflight should pass and the **sysperf window should open**.
   That's the whole test. If it opens, issue2 is done.
3. (already known-green, fine to re-confirm) build reaches 100%, `ctest` 7/7.

### Non-blocking noise in your issue2 log — safe to ignore
- `Could NOT find XKB (missing: XKB_LIBRARY ...)` — cosmetic CMake/Qt message; build still completes.
- `[WARN] release.json missing (generated at start)` — expected; `version.sh` writes it at launch.

---

PS — passing notes back and forth as `issueN.md` files committed into the repo is a pretty
weird way to hold a conversation :D it works, but it's basically using git as a very slow
chat. A real GitHub Issue or a PR thread would thread all this properly. Happy to keep doing
it this way if you like it, just flagging it.
