#include "collector/meminfo.h"

#include "collector/proc_util.h"

namespace sysperf {

std::map<std::string, uint64_t> MemCollector::parseText(const std::string& text) {
    std::map<std::string, uint64_t> kv;
    for (const auto& line : procutil::splitLines(text)) {
        auto colon = line.find(':');
        if (colon == std::string::npos) continue;
        std::string key = line.substr(0, colon);
        auto toks = procutil::tokenize(line.substr(colon + 1));
        if (toks.empty()) continue;
        // toks[0] = number, toks[1] = "kB" (if present). /proc/meminfo is in kB.
        kv[key] = procutil::toU64(toks[0]);
    }
    return kv;
}

MemInfo MemCollector::fromMap(const std::map<std::string, uint64_t>& kv) {
    auto get = [&](const char* k) -> uint64_t {
        auto it = kv.find(k);
        return it == kv.end() ? 0ULL : it->second;
    };
    constexpr uint64_t KB = 1024ULL;  // /proc/meminfo values are KiB

    const uint64_t total = get("MemTotal");
    const uint64_t avail = get("MemAvailable");
    const uint64_t cached = get("Cached");
    const uint64_t buffers = get("Buffers");
    const uint64_t sRecl = get("SReclaimable");
    const uint64_t sUnrecl = get("SUnreclaim");

    MemInfo m;
    m.total = total * KB;
    m.available = avail * KB;
    // "In use" is total minus what's actually reclaimable/available — the honest figure.
    m.used = (total > avail ? (total - avail) : 0ULL) * KB;
    m.free = get("MemFree") * KB;
    m.cached = (cached + buffers + sRecl) * KB;
    m.buffers = buffers * KB;
    m.s_reclaimable = sRecl * KB;
    m.s_unreclaim = sUnrecl * KB;
    m.committed = get("Committed_AS") * KB;
    m.commit_limit = get("CommitLimit") * KB;
    m.swap_total = get("SwapTotal") * KB;
    m.swap_free = get("SwapFree") * KB;
    return m;
}

MemInfo MemCollector::update(const std::string& procRoot) {
    std::string text;
    if (!procutil::readFile(procutil::join(procRoot, "meminfo"), text)) return {};
    return fromMap(parseText(text));
}

} // namespace sysperf
