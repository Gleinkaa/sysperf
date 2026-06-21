// snapshot.h — the immutable value type the worker thread emits by value (queued
// signal) to the GUI thread. Pure C++ (NO Qt) so the core library and tests stay
// Qt-free; the Qt metatype declaration lives in model/snapshot_qt.h.
//
// Design rule (see docs/ARCHITECTURE.md): the worker sends only the LATEST derived
// sample (a few scalars + small vectors). The rolling history (ring buffers) lives
// on the GUI side. This keeps the cross-thread payload tiny.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace sysperf {

// ── CPU ────────────────────────────────────────────────────────────────────────
struct CpuInfo {
    double total_pct = 0.0;            // aggregate utilisation, 0..100
    std::vector<double> per_core_pct;  // one entry per logical CPU, 0..100
    double freq_ghz = 0.0;             // mean current frequency across cores (0 = unknown)
    std::string model;                 // /proc/cpuinfo "model name" (static; generic per host)
    int logical_cores = 0;
};

// ── Memory (all byte counts; honest Linux semantics) ────────────────────────────
struct MemInfo {
    uint64_t total = 0;          // MemTotal
    uint64_t available = 0;      // MemAvailable
    uint64_t used = 0;           // total - available  (NOT total - free)
    uint64_t free = 0;           // MemFree (shown for completeness, not "in use")
    uint64_t cached = 0;         // Cached + Buffers + SReclaimable
    uint64_t buffers = 0;        // Buffers
    uint64_t s_reclaimable = 0;  // SReclaimable  (honest analog of "paged pool")
    uint64_t s_unreclaim = 0;    // SUnreclaim    (honest analog of "non-paged pool")
    uint64_t committed = 0;      // Committed_AS
    uint64_t commit_limit = 0;   // CommitLimit
    uint64_t swap_total = 0;     // SwapTotal
    uint64_t swap_free = 0;      // SwapFree
};

// ── Disk ────────────────────────────────────────────────────────────────────────
struct DiskSample {
    std::string name;
    double util_pct = 0.0;   // io_ticks delta / measured dt, clamped 0..100
    double read_bps = 0.0;   // bytes/sec
    double write_bps = 0.0;  // bytes/sec
    bool rotational = false; // best-effort: true=HDD, false=SSD/NVMe
    bool has_rotational = false; // whether rotational could be determined
};

// ── Network ──────────────────────────────────────────────────────────────────────
struct NetSample {
    std::string name;
    double rx_bps = 0.0;   // bytes/sec received
    double tx_bps = 0.0;   // bytes/sec sent
    long speed_mbps = -1;  // link speed (Mbps); -1 = unknown
};

// ── GPU ──────────────────────────────────────────────────────────────────────────
enum class GpuVendor { Unknown, Nvidia, Amd, Intel };

struct GpuSample {
    std::string name;
    GpuVendor vendor = GpuVendor::Unknown;
    double util_pct = -1.0;   // -1 = unavailable
    uint64_t mem_used = 0;    // bytes
    uint64_t mem_total = 0;   // bytes
    double temp_c = -1.0;     // -1 = unavailable
    double clock_mhz = -1.0;  // -1 = unavailable
    double power_w = -1.0;    // -1 = unavailable
    std::string source;       // "nvml" | "fdinfo" | "sysfs"
    std::string id;           // stable key (PCI addr or nvml index) for UI ring buffers
};

// ── Self-instrumentation (the "measure-first" budget guardrail) ─────────────────
struct SelfMetrics {
    uint64_t pss_bytes = 0;   // /proc/self/smaps_rollup Pss (honest; RSS double-counts Qt)
    double cpu_pct = 0.0;     // own CPU as % of one core, over measured dt
    bool over_budget = false; // pss/cpu exceeded the soft alarm this tick
};

// ── The snapshot ─────────────────────────────────────────────────────────────────
struct Snapshot {
    double mono_time = 0.0;  // CLOCK_MONOTONIC seconds at sample time
    double dt = 0.0;         // measured seconds since previous sample (0 on the first)
    bool first = true;       // true ⇒ deltas are not yet meaningful

    CpuInfo cpu;
    MemInfo mem;
    std::vector<DiskSample> disks;
    std::vector<NetSample> nets;
    std::vector<GpuSample> gpus;
    SelfMetrics self;
};

} // namespace sysperf
