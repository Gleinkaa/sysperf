// gpu_nvml.h — NVIDIA GPU metrics via NVML, loaded with dlopen at RUNTIME.
//
// We deliberately do NOT link libnvidia-ml. Hard-linking it makes the whole app
// fail to start on every machine without the NVIDIA driver (all AMD/Intel-only
// boxes). If libnvidia-ml.so.1 is absent, or NVML init fails, this collector
// simply reports zero GPUs and the panel is omitted.
//
// The handful of NVML symbols/structs we need are declared locally (stable ABI),
// so no NVML -dev headers are required to build.
#pragma once

#include <string>
#include <vector>

#include "model/snapshot.h"

namespace sysperf {

class NvmlGpu {
public:
    NvmlGpu() = default;
    ~NvmlGpu();
    NvmlGpu(const NvmlGpu&) = delete;
    NvmlGpu& operator=(const NvmlGpu&) = delete;

    // Attempts dlopen + nvmlInit on first use. Safe to call when no NVIDIA driver
    // is present (returns an empty vector). Never throws.
    std::vector<GpuSample> sample();

    bool available() const { return ok_; }

private:
    bool ensureLoaded();
    void unload();

    void* lib_ = nullptr;     // dlopen handle
    bool tried_ = false;      // attempted to load already
    bool ok_ = false;         // NVML usable
    bool initialised_ = false;// nvmlInit succeeded (so we must nvmlShutdown)

    // Resolved function pointers (typed in the .cpp).
    void* fn_init_ = nullptr;
    void* fn_shutdown_ = nullptr;
    void* fn_count_ = nullptr;
    void* fn_handle_ = nullptr;
    void* fn_util_ = nullptr;
    void* fn_mem_ = nullptr;
    void* fn_name_ = nullptr;
    void* fn_temp_ = nullptr;
    void* fn_clock_ = nullptr;
    void* fn_power_ = nullptr;
};

} // namespace sysperf
