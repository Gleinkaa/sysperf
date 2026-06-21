// self_metrics.h — the "measure-first" budget guardrail. Reads the app's OWN
// footprint every tick so the monitor can prove it isn't the thing consuming the
// resources it measures.
//   PSS  from /proc/self/smaps_rollup  (honest — RSS double-counts shared Qt libs)
//   CPU  from /proc/self/stat utime+stime, as % of ONE core over measured dt
#pragma once

#include <string>

#include "model/snapshot.h"

namespace sysperf {

struct BudgetLimits {
    double pss_mb_warn = 50.0;   // soft alarm threshold
    double cpu_pct_warn = 1.0;   // % of one core
};

class SelfCollector {
public:
    // Parse the Pss line (kB) out of a smaps_rollup body. Returns 0 if absent.
    static uint64_t parsePssKb(const std::string& smapsRollup);

    // Parse (utime + stime) in clock ticks out of a /proc/self/stat body.
    static uint64_t parseCpuTicks(const std::string& statBody);

    // Stateful update: reads procRoot/self, computes PSS bytes + CPU% over dt, and
    // sets over_budget against `limits`. `clkTck` is sysconf(_SC_CLK_TCK).
    SelfMetrics update(const std::string& procRoot, double dt, const BudgetLimits& limits,
                       long clkTck);

private:
    bool hasPrev_ = false;
    uint64_t prevTicks_ = 0;
};

} // namespace sysperf
