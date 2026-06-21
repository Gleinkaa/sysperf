// gpu_fdinfo.h — vendor-neutral GPU utilisation from DRM fdinfo counters.
//
// The kernel exposes per-open-file engine busy-time in /proc/<pid>/fdinfo/<fd> as
//   drm-engine-<name>:  <nanoseconds> ns
// We sum each engine's ns across all distinct DRM clients of a device (deduped by
// drm-client-id so shared fds aren't double-counted), then derive utilisation as
//   util_engine = (engine_ns delta) / (measured dt in ns) * 100
// and report the busiest engine as the GPU's utilisation. This is the same
// unprivileged method nvtop/btop use — but this is an independent implementation
// (pure file parsing, no libdrm linkage, no GPL'd code copied).
//
// This is the heaviest collector (a per-process scan), which is acceptable at 1 Hz.
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "model/snapshot.h"

namespace sysperf {

// Parsed contents of a single fdinfo file (only the DRM-relevant lines).
struct FdinfoClient {
    bool is_drm = false;
    std::string driver;                       // drm-driver (e.g. amdgpu, i915, xe)
    std::string pdev;                          // drm-pdev (PCI addr) — device identity
    uint64_t client_id = 0;
    bool has_client_id = false;
    std::map<std::string, uint64_t> engines;  // drm-engine-<name> → busy ns
};

// Per-device engine totals plus a representative driver string.
struct DrmDeviceTotals {
    std::string driver;
    std::map<std::string, uint64_t> engines;  // engine → summed ns this scan
};

class FdinfoGpu {
public:
    // Pure parse of one fdinfo body.
    static FdinfoClient parseFdinfo(const std::string& text);

    // util% for one engine between two ns readings over dt seconds.
    static double engineUtil(uint64_t prevNs, uint64_t curNs, double dt);

    // Scan /proc/<pid>/fdinfo/* under procRoot and aggregate engine ns per device,
    // deduping clients by drm-client-id. Pure-ish (filesystem only); testable by
    // pointing procRoot at a fixture tree.
    static std::map<std::string, DrmDeviceTotals> scan(const std::string& procRoot);

    // Stateful update: scan, diff against the previous totals, enrich with sysfs
    // (temperature) and emit one GpuSample per DRM device. NVIDIA devices are left
    // to the NVML collector and skipped here when their driver string is "nvidia".
    std::vector<GpuSample> update(const std::string& procRoot, const std::string& sysRoot,
                                  double dt);

private:
    bool hasPrev_ = false;
    std::map<std::string, DrmDeviceTotals> prev_;
};

} // namespace sysperf
