#include "collector/cpu.h"

#include <algorithm>
#include <cctype>

#include "collector/proc_util.h"

namespace sysperf {

using procutil::tokenize;

namespace {

// Parse one "cpu..." line's numeric fields into CpuTimes. Missing trailing fields
// (older kernels) default to 0.
CpuTimes parseCpuLine(const std::vector<std::string>& t) {
    CpuTimes c;
    auto at = [&](std::size_t i) -> uint64_t {
        return (i < t.size()) ? procutil::toU64(t[i]) : 0ULL;
    };
    // t[0] is the label ("cpu" / "cpuN"); numeric fields start at t[1].
    c.user       = at(1);
    c.nice       = at(2);
    c.system     = at(3);
    c.idle       = at(4);
    c.iowait     = at(5);
    c.irq        = at(6);
    c.softirq    = at(7);
    c.steal      = at(8);
    c.guest      = at(9);
    c.guest_nice = at(10);
    return c;
}

// Returns the core index for a label like "cpu0", or -1 if it's the aggregate
// "cpu" line, or -2 if it's not a per-core cpu line.
int coreIndex(const std::string& label) {
    if (label.rfind("cpu", 0) != 0) return -2;
    if (label.size() == 3) return -1;  // exactly "cpu"
    for (std::size_t i = 3; i < label.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(label[i]))) return -2;
    }
    return static_cast<int>(procutil::toLong(label.substr(3)));
}

} // namespace

ParsedStat CpuCollector::parseStatText(const std::string& text) {
    ParsedStat ps;
    for (const auto& line : procutil::splitLines(text)) {
        if (line.rfind("cpu", 0) != 0) continue;  // cpu lines are first; bail-safe below
        auto t = tokenize(line);
        if (t.empty()) continue;
        int idx = coreIndex(t[0]);
        if (idx == -1) {
            ps.aggregate = parseCpuLine(t);
            ps.ok = true;
        } else if (idx >= 0) {
            if (static_cast<int>(ps.cores.size()) <= idx) ps.cores.resize(idx + 1);
            ps.cores[idx] = parseCpuLine(t);
        }
    }
    return ps;
}

ParsedStat CpuCollector::readStat(const std::string& procRoot) {
    std::string text;
    if (!procutil::readFile(procutil::join(procRoot, "stat"), text)) return {};
    return parseStatText(text);
}

double CpuCollector::utilization(const CpuTimes& prev, const CpuTimes& cur) {
    const uint64_t totPrev = prev.total();
    const uint64_t totCur = cur.total();
    if (totCur <= totPrev) return 0.0;  // counter reset / no progress
    const uint64_t totDelta = totCur - totPrev;
    const uint64_t idlePrev = prev.idleAll();
    const uint64_t idleCur = cur.idleAll();
    const uint64_t idleDelta = (idleCur > idlePrev) ? (idleCur - idlePrev) : 0ULL;
    double busyDelta = static_cast<double>(totDelta) - static_cast<double>(idleDelta);
    if (busyDelta < 0.0) busyDelta = 0.0;
    double pct = 100.0 * busyDelta / static_cast<double>(totDelta);
    if (pct < 0.0) pct = 0.0;
    if (pct > 100.0) pct = 100.0;
    return pct;
}

std::string CpuCollector::readModel(const std::string& procRoot) {
    std::string text;
    if (!procutil::readFile(procutil::join(procRoot, "cpuinfo"), text)) return {};
    for (const auto& line : procutil::splitLines(text)) {
        // Match "model name" (x86) or fall back to "Model" (some arm).
        auto colon = line.find(':');
        if (colon == std::string::npos) continue;
        std::string key = line.substr(0, colon);
        // trim
        while (!key.empty() && std::isspace(static_cast<unsigned char>(key.back()))) key.pop_back();
        if (key == "model name" || key == "Model") {
            std::string val = line.substr(colon + 1);
            std::size_t s = val.find_first_not_of(" \t");
            if (s != std::string::npos) val = val.substr(s);
            return val;
        }
    }
    return {};
}

double CpuCollector::readMeanGhz(const std::string& sysRoot, int logicalCores) {
    if (logicalCores <= 0) return 0.0;
    double sumKhz = 0.0;
    int counted = 0;
    for (int i = 0; i < logicalCores; ++i) {
        std::string p = procutil::join(
            sysRoot, "devices/system/cpu/cpu" + std::to_string(i) + "/cpufreq/scaling_cur_freq");
        long khz = procutil::readLong(p, -1);
        if (khz > 0) {
            sumKhz += static_cast<double>(khz);
            ++counted;
        }
    }
    if (counted == 0) return 0.0;
    return (sumKhz / counted) / 1.0e6;  // kHz → GHz
}

CpuInfo CpuCollector::update(const std::string& procRoot, const std::string& sysRoot) {
    CpuInfo info;
    ParsedStat cur = readStat(procRoot);
    info.logical_cores = static_cast<int>(cur.cores.size());

    if (!modelCached_) {
        cachedModel_ = readModel(procRoot);
        modelCached_ = true;
    }
    info.model = cachedModel_;
    info.freq_ghz = readMeanGhz(sysRoot, info.logical_cores);

    if (hasPrev_ && cur.ok && prev_.ok) {
        info.total_pct = utilization(prev_.aggregate, cur.aggregate);
        const std::size_t n = std::min(prev_.cores.size(), cur.cores.size());
        info.per_core_pct.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            info.per_core_pct.push_back(utilization(prev_.cores[i], cur.cores[i]));
        }
    } else {
        info.per_core_pct.assign(cur.cores.size(), 0.0);
    }

    prev_ = std::move(cur);
    hasPrev_ = prev_.ok;
    return info;
}

} // namespace sysperf
