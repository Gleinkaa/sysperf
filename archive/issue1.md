# Issue 1 — onboard GPU VRAM shows always 0 / "—"

## Status: ✅ FIXED (2026-06-21)

## Root cause
The fdinfo GPU collector (`src/collector/gpu_fdinfo.cpp`) never attempted to read VRAM
at all. It only collected utilisation (engine busy-ns) and temperature (hwmon).
The `mem_total` and `mem_used` fields of `GpuSample` stayed at their struct default (0),
so the UI showed `"—"` for the Memory stat on all non-NVIDIA GPUs.

## Fix
AMD/Intel DRM drivers expose VRAM info via standard PCI device sysfs attributes:
- `/sys/bus/pci/devices/<pdev>/mem_info_vram_total` — total VRAM (bytes)
- `/sys/bus/pci/devices/<pdev>/mem_info_vram_used`  — VRAM in use (bytes)

Added reading of these files in `FdinfoGpu::update()`, right after the temperature read.
`amdgpu` always publishes these attributes; `i915` (Intel) may not (iGPU shares system
RAM, kernel doesn't always expose `mem_info_vram_*`). When the files are absent or total
is 0, the stat gracefully remains `"—"`.

## Verification (on Linux with AMD or Intel iGPU)
```
git pull
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j8
./scripts/start.sh
```
→ GPU panel "Memory" stat should show used/total for AMD GPUs
→ Intel iGPU may still show "—" if kernel driver doesn't expose mem_info_vram_*
