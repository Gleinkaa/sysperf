#!/usr/bin/env bash
# preflight.sh — the bouncer. Exits non-zero on any drift. `start.sh` runs this
# first; never bypass. Adapted for a DESKTOP GUI (no port, no /api/version): the
# web-service checks are replaced with toolchain + source + "don't hard-link
# NVML/libdrm" + "single unprivileged process" guards.

set -euo pipefail
cd "$(dirname "$0")/.."   # project root

FAIL=0
PROJECT="$(basename "$PWD")"
echo "=== preflight: $PROJECT (desktop GUI) ==="

ok()   { echo "  [OK]   $1"; }
warn() { echo "  [WARN] $1"; }
fail() { echo "  [FAIL] $1"; FAIL=1; }

# ─── GENERIC CHECKS (the mandatory template shape) ────────────────────────────

# 1. version.txt exists and is semver-shaped
if [[ ! -f version.txt ]]; then
  fail "version.txt missing"
else
  VER=$(tr -d '[:space:]' < version.txt)
  if [[ "$VER" =~ ^[0-9]+\.[0-9]+\.[0-9]+(-[A-Za-z0-9.-]+)?$ ]]; then
    ok "version.txt = $VER"
  else
    fail "version.txt is not semver: '$VER'"
  fi
fi

# 2. STATUS.md says active
if [[ -f STATUS_FROZEN.md ]]; then
  fail "STATUS_FROZEN.md exists — this project is frozen."
elif [[ -f STATUS.md ]]; then
  grep -qi "^# Status: active" STATUS.md && ok "STATUS = active" || fail "STATUS.md not active"
else
  fail "STATUS.md missing"
fi

# 3. Mandatory docs
[[ -f DEPLOYMENT.md ]] && ok "DEPLOYMENT.md present" || fail "DEPLOYMENT.md missing"
[[ -f README.md ]]     && ok "README.md present"     || fail "README.md missing"
if [[ -f CLAUDE.md ]]; then
  LINES=$(wc -l < CLAUDE.md)
  ok "CLAUDE.md present ($LINES lines)"
else
  fail "CLAUDE.md missing"
fi

# 4. .env.example exists; all keys present in env or .env (sysperf needs none, but
#    the standard wants the schema validated).
[[ -f .env.example ]] || fail ".env.example missing"
if [[ -f .env.example ]]; then
  MISSING=""
  while IFS= read -r line; do
    [[ "$line" =~ ^[[:space:]]*# ]] && continue
    [[ -z "${line// }" ]] && continue
    KEY="${line%%=*}"; KEY="${KEY// }"
    [[ -z "$KEY" ]] && continue
    DEFAULT="${line#*=}"
    # Optional keys (have a default value in .env.example) don't need to be set.
    if [[ -z "${!KEY:-}" ]]; then
      if [[ -f .env ]] && grep -q "^${KEY}=" .env; then :;
      elif [[ -n "$DEFAULT" ]]; then :;   # documented default ⇒ optional
      else MISSING="$MISSING $KEY"; fi
    fi
  done < .env.example
  [[ -z "$MISSING" ]] && ok "all required .env.example keys satisfied" \
                       || fail "missing required env vars:$MISSING"
fi

# 5. config/default.json is valid JSON
if [[ -f config/default.json ]]; then
  if command -v python3 >/dev/null 2>&1; then PY=python3;
  elif command -v python >/dev/null 2>&1; then PY=python; else PY=""; fi
  if [[ -n "$PY" ]]; then
    if "$PY" -c "import json,sys; json.load(open('config/default.json'))" 2>/dev/null; then
      ok "config/default.json is valid JSON"
    else
      fail "config/default.json is not valid JSON"
    fi
  else
    warn "no python to validate config/default.json"
  fi
else
  fail "config/default.json missing"
fi

# 6. release.json present or regenerable
[[ -f release.json ]] && ok "release.json present" || warn "release.json missing (generated at start)"

# 7. No banned literals
BANNED='Dandelion21|musikant1#|password=.{1,20}[\x27"][^\x27"]+[\x27"]'
if grep -rIE "$BANNED" \
     --exclude-dir=build --exclude-dir=build-debug-asan --exclude-dir=build-core \
     --exclude-dir=.git --exclude='preflight.sh' . 2>/dev/null | head -5 | grep -q .; then
  fail "banned literal found"
else
  ok "no banned literals"
fi

# ─── PROJECT-SPECIFIC CHECKS (desktop C++/Qt6) ────────────────────────────────
echo "--- desktop GUI checks ---"

# 8. Build toolchain
command -v cmake >/dev/null 2>&1 && ok "cmake: $(cmake --version | head -1)" \
                                 || fail "cmake not found (apt install cmake)"
if command -v g++ >/dev/null 2>&1; then ok "g++ present";
elif command -v c++ >/dev/null 2>&1; then ok "c++ present";
elif command -v clang++ >/dev/null 2>&1; then ok "clang++ present";
else fail "no C++ compiler (apt install build-essential)"; fi

# 9. Source tree present
[[ -f CMakeLists.txt ]] && ok "CMakeLists.txt present" || fail "CMakeLists.txt missing"
[[ -f src/main.cpp ]]   && ok "src/main.cpp present"   || fail "src/main.cpp missing"
[[ -d tests ]]          && ok "tests/ present"         || warn "tests/ missing"

# 10. NVML / libdrm must NOT be hard-linked (graceful degradation invariant).
if grep -REn 'target_link_libraries' CMakeLists.txt | grep -Eiq 'nvidia-ml|libnvidia|[^a-z]drm[^a-z]|libdrm'; then
  fail "CMakeLists hard-links NVML/libdrm — must dlopen NVML and parse fdinfo as files"
else
  ok "NVML/libdrm not hard-linked (dlopen only)"
fi

# 11. Single unprivileged process: no root/cap/polkit/D-Bus-helper machinery.
if grep -rIEn 'pkexec|polkit|CAP_SYS_ADMIN|setcap|sd_bus_|dbus.*helper' \
     --include='*.cpp' --include='*.h' src 2>/dev/null | grep -q .; then
  fail "found privilege-escalation machinery — scope is single unprivileged process"
else
  ok "no privilege-escalation machinery (single unprivileged process)"
fi

# ─── RESULT ───────────────────────────────────────────────────────────────────
if [[ "$FAIL" -ne 0 ]]; then
  echo "=== preflight FAILED ==="
  exit 1
fi
echo "=== preflight OK ==="
exit 0
