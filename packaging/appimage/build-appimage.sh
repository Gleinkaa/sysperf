#!/usr/bin/env bash
# build-appimage.sh — produce a portable sysperf AppImage.
#
# Build this on the OLDEST supported glibc/Ubuntu (e.g. 22.04) so the result runs
# on that release and everything newer. linuxdeploy + its Qt plugin bundle the Qt
# platform plugin and platformtheme. NVML and libdrm are intentionally NOT bundled:
# the app dlopen's NVML at runtime and reads DRM fdinfo as files, so it degrades
# gracefully on machines without a given GPU vendor.
#
# Snap/Flatpak are deliberately NOT provided: strict confinement blocks other
# processes' /proc, fdinfo and device nodes, which would silently break the
# cross-process and GPU collectors.

set -euo pipefail
cd "$(dirname "$0")/../.."   # project root

BUILD="build-appimage"
APPDIR="$PWD/packaging/appimage/AppDir"
TOOLS="$PWD/packaging/appimage/tools"

echo "--- build (Release, no tests) ---"
cmake -S . -B "$BUILD" -DCMAKE_BUILD_TYPE=Release -DSYSPERF_BUILD_TESTS=OFF
cmake --build "$BUILD" -j"$(nproc 2>/dev/null || echo 2)"

echo "--- stage AppDir ---"
rm -rf "$APPDIR"
DESTDIR="$APPDIR" cmake --install "$BUILD" --prefix /usr

echo "--- fetch linuxdeploy tools ---"
mkdir -p "$TOOLS"
fetch() {
  local out="$TOOLS/$1"
  if [[ ! -f "$out" ]]; then
    wget -q -O "$out" "$2" || { echo "download failed: $2"; exit 1; }
    chmod +x "$out"
  fi
}
fetch linuxdeploy-x86_64.AppImage \
  https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
fetch linuxdeploy-plugin-qt-x86_64.AppImage \
  https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

echo "--- bundle + produce AppImage ---"
export QMAKE="${QMAKE:-$(command -v qmake6 || true)}"
"$TOOLS/linuxdeploy-x86_64.AppImage" \
  --appdir "$APPDIR" \
  --plugin qt \
  --output appimage \
  --desktop-file "$APPDIR/usr/share/applications/sysperf.desktop" \
  --icon-file "$PWD/packaging/sysperf.svg"

mv -f sysperf*.AppImage packaging/appimage/ 2>/dev/null || true
echo "AppImage written under packaging/appimage/"
