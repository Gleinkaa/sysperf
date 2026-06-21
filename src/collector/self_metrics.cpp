#include "collector/self_metrics.h"

#include "collector/proc_util.h"

namespace sysperf {

uint64_t SelfCollector::parsePssKb(const std::string& smapsRollup) {
    for (const auto& line : procutil::splitLines(smapsRollup)) {
        if (line.rfind("Pss:", 0) == 0) {
            auto toks = procutil::tokenize(line);
            // "Pss:  12345 kB"
            if (toks.size() >= 2) return procutil::toU64(toks[1]);
        }
    }
    return 0;
}

uint64_t SelfCollector::parseCpuTicks(const std::string& statBody) {
    // /proc/self/stat: pid (comm) state ... The comm field is parenthesised and may
    // contain spaces, so anchor parsing AFTER the last ')'.
    auto rp = statBody.rfind(')');
    if (rp == std::string::npos) return 0;
    std::string rest = statBody.substr(rp + 1);
    auto toks = procutil::tokenize(rest);
    // After "(comm)", token[0] is field 3 (state). Fields (1-based in the man page):
    //   14 utime, 15 stime  → indices 11 and 12 of `toks` (field N → toks[N-3]).
    if (toks.size() < 13) return 0;
    uint64_t utime = procutil::toU64(toks[11]);
    uint64_t stime = procutil::toU64(toks[12]);
    return utime + stime;
}

SelfMetrics SelfCollector::update(const std::string& procRoot, double dt,
                                  const BudgetLimits& limits, long clkTck) {
    SelfMetrics m;

    std::string rollup;
    if (procutil::readFile(procutil::join(procRoot, "self/smaps_rollup"), rollup)) {
        m.pss_bytes = parsePssKb(rollup) * 1024ULL;
    }

    std::string stat;
    if (procutil::readFile(procutil::join(procRoot, "self/stat"), stat)) {
        uint64_t ticks = parseCpuTicks(stat);
        if (hasPrev_ && dt > 0.0 && clkTck > 0) {
            double dticks = (ticks >= prevTicks_) ? static_cast<double>(ticks - prevTicks_) : 0.0;
            m.cpu_pct = 100.0 * (dticks / static_cast<double>(clkTck)) / dt;
            if (m.cpu_pct < 0.0) m.cpu_pct = 0.0;
        }
        prevTicks_ = ticks;
        hasPrev_ = true;
    }

    const double pssMb = static_cast<double>(m.pss_bytes) / (1024.0 * 1024.0);
    m.over_budget = (pssMb > limits.pss_mb_warn) || (m.cpu_pct > limits.cpu_pct_warn);
    return m;
}

} // namespace sysperf
