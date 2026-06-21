#include "collector/gpu_nvml.h"

#include <dlfcn.h>

#include <cstring>

namespace sysperf {

namespace {

// ── Minimal NVML ABI (stable across driver versions) ────────────────────────────
// nvmlReturn_t is an int enum; 0 == NVML_SUCCESS.
using nvmlReturn_t = int;
constexpr nvmlReturn_t NVML_SUCCESS = 0;

using nvmlDevice_t = void*;

struct nvmlUtilization_t {
    unsigned int gpu;     // % of time over the past sample period that any kernel ran
    unsigned int memory;  // % of time over the past sample period that memory was r/w
};

struct nvmlMemory_t {
    unsigned long long total;
    unsigned long long free;
    unsigned long long used;
};

// Temperature sensor / clock-type enums we use.
constexpr int NVML_TEMPERATURE_GPU = 0;
constexpr int NVML_CLOCK_GRAPHICS = 0;

using fn_init_t = nvmlReturn_t (*)();
using fn_shutdown_t = nvmlReturn_t (*)();
using fn_count_t = nvmlReturn_t (*)(unsigned int*);
using fn_handle_t = nvmlReturn_t (*)(unsigned int, nvmlDevice_t*);
using fn_util_t = nvmlReturn_t (*)(nvmlDevice_t, nvmlUtilization_t*);
using fn_mem_t = nvmlReturn_t (*)(nvmlDevice_t, nvmlMemory_t*);
using fn_name_t = nvmlReturn_t (*)(nvmlDevice_t, char*, unsigned int);
using fn_temp_t = nvmlReturn_t (*)(nvmlDevice_t, int, unsigned int*);
using fn_clock_t = nvmlReturn_t (*)(nvmlDevice_t, int, unsigned int*);
using fn_power_t = nvmlReturn_t (*)(nvmlDevice_t, unsigned int*);

template <typename T>
T cast(void* p) { return reinterpret_cast<T>(p); }

} // namespace

NvmlGpu::~NvmlGpu() { unload(); }

void NvmlGpu::unload() {
    if (initialised_ && fn_shutdown_) {
        cast<fn_shutdown_t>(fn_shutdown_)();
        initialised_ = false;
    }
    if (lib_) {
        ::dlclose(lib_);
        lib_ = nullptr;
    }
    ok_ = false;
}

bool NvmlGpu::ensureLoaded() {
    if (tried_) return ok_;
    tried_ = true;

    // Try the SONAME first (what's actually shipped), then the dev symlink.
    const char* candidates[] = {"libnvidia-ml.so.1", "libnvidia-ml.so"};
    for (const char* name : candidates) {
        lib_ = ::dlopen(name, RTLD_NOW | RTLD_LOCAL);
        if (lib_) break;
    }
    if (!lib_) return false;  // no NVIDIA driver here — perfectly normal

    // Prefer the versioned (_v2) entry points; fall back to the unversioned ones.
    auto sym = [&](const char* a, const char* b) -> void* {
        void* p = ::dlsym(lib_, a);
        return p ? p : ::dlsym(lib_, b);
    };

    fn_init_ = sym("nvmlInit_v2", "nvmlInit");
    fn_shutdown_ = sym("nvmlShutdown", "nvmlShutdown");
    fn_count_ = sym("nvmlDeviceGetCount_v2", "nvmlDeviceGetCount");
    fn_handle_ = sym("nvmlDeviceGetHandleByIndex_v2", "nvmlDeviceGetHandleByIndex");
    fn_util_ = sym("nvmlDeviceGetUtilizationRates", "nvmlDeviceGetUtilizationRates");
    fn_mem_ = sym("nvmlDeviceGetMemoryInfo", "nvmlDeviceGetMemoryInfo");
    fn_name_ = sym("nvmlDeviceGetName", "nvmlDeviceGetName");
    fn_temp_ = sym("nvmlDeviceGetTemperature", "nvmlDeviceGetTemperature");
    fn_clock_ = sym("nvmlDeviceGetClockInfo", "nvmlDeviceGetClockInfo");
    fn_power_ = sym("nvmlDeviceGetPowerUsage", "nvmlDeviceGetPowerUsage");

    if (!fn_init_ || !fn_count_ || !fn_handle_) {  // the bare minimum
        unload();
        return false;
    }
    if (cast<fn_init_t>(fn_init_)() != NVML_SUCCESS) {
        unload();
        return false;
    }
    initialised_ = true;
    ok_ = true;
    return true;
}

std::vector<GpuSample> NvmlGpu::sample() {
    std::vector<GpuSample> out;
    if (!ensureLoaded()) return out;

    unsigned int count = 0;
    if (cast<fn_count_t>(fn_count_)(&count) != NVML_SUCCESS) return out;

    for (unsigned int i = 0; i < count; ++i) {
        nvmlDevice_t dev = nullptr;
        if (cast<fn_handle_t>(fn_handle_)(i, &dev) != NVML_SUCCESS || !dev) continue;

        GpuSample g;
        g.vendor = GpuVendor::Nvidia;
        g.source = "nvml";
        g.id = "nvml:" + std::to_string(i);
        g.name = "NVIDIA GPU " + std::to_string(i);

        if (fn_name_) {
            char buf[96] = {0};
            if (cast<fn_name_t>(fn_name_)(dev, buf, sizeof(buf) - 1) == NVML_SUCCESS && buf[0]) {
                g.name = buf;
            }
        }
        if (fn_util_) {
            nvmlUtilization_t u{};
            if (cast<fn_util_t>(fn_util_)(dev, &u) == NVML_SUCCESS) {
                g.util_pct = static_cast<double>(u.gpu);
            }
        }
        if (fn_mem_) {
            nvmlMemory_t m{};
            if (cast<fn_mem_t>(fn_mem_)(dev, &m) == NVML_SUCCESS) {
                g.mem_total = m.total;
                g.mem_used = m.used;
            }
        }
        if (fn_temp_) {
            unsigned int t = 0;
            if (cast<fn_temp_t>(fn_temp_)(dev, NVML_TEMPERATURE_GPU, &t) == NVML_SUCCESS)
                g.temp_c = static_cast<double>(t);
        }
        if (fn_clock_) {
            unsigned int c = 0;
            if (cast<fn_clock_t>(fn_clock_)(dev, NVML_CLOCK_GRAPHICS, &c) == NVML_SUCCESS)
                g.clock_mhz = static_cast<double>(c);
        }
        if (fn_power_) {
            unsigned int mw = 0;
            if (cast<fn_power_t>(fn_power_)(dev, &mw) == NVML_SUCCESS)
                g.power_w = static_cast<double>(mw) / 1000.0;
        }
        out.push_back(std::move(g));
    }
    return out;
}

} // namespace sysperf
