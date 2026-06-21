// cpu.h — CPU utilisation (total + per-core), current frequency, model.
// Source: /proc/stat jiffy deltas; /sys/.../scaling_cur_freq; /proc/cpuinfo.
#pragma once

#include <string>
#include <vector>

#include "model/snapshot.h"

namespace sysperf {

// Raw jiffy counters for one cpu line in /proc/stat.
struct CpuTimes {
    uint64_t user = 0, nice = 0, system = 0, idle = 0, iowait = 0;
    uint64_t irq = 0, softirq = 0, steal = 0, guest = 0, guest_nice = 0;

    // idle component (idle + iowait).
    uint64_t idleAll() const { return idle + iowait; }
    // busy component. guest/guest_nice are already counted inside user/nice by the
    // kernel, so they are NOT added again here.
    uint64_t busy() const { return user + nice + system + irq + softirq + steal; }
    uint64_t total() const { return idleAll() + busy(); }
};

struct ParsedStat {
    CpuTimes aggregate;            // the "cpu " line
    std::vector<CpuTimes> cores;   // cpu0, cpu1, ... in index order
    bool ok = false;
};

class CpuCollector {
public:
    // Pure parse of a /proc/stat body.
    static ParsedStat parseStatText(const std::string& text);
    // Read + parse /proc/stat under `procRoot`.
    static ParsedStat readStat(const std::string& procRoot);

    // Utilisation between two raw samples: 100 * busyDelta / totalDelta, clamped.
    // Note: this ratio is intrinsically normalised by the (shared) sampling
    // interval, so it does NOT need the measured dt — both numerator and
    // denominator are accumulated over the same wall-clock window.
    static double utilization(const CpuTimes& prev, const CpuTimes& cur);

    // Static info (filled lazily, cached): model name + mean current GHz.
    static std::string readModel(const std::string& procRoot);
    static double readMeanGhz(const std::string& sysRoot, int logicalCores);

    // Stateful update: reads procRoot/sysRoot, diffs against the stored previous
    // sample, fills CpuInfo. First call returns zero utilisation (no delta yet).
    CpuInfo update(const std::string& procRoot, const std::string& sysRoot);

private:
    bool hasPrev_ = false;
    ParsedStat prev_;
    std::string cachedModel_;
    bool modelCached_ = false;
};

} // namespace sysperf
