#include "collector/netdev.h"

#include "collector/proc_util.h"

namespace sysperf {

std::vector<NetRaw> NetCollector::parseText(const std::string& text) {
    std::vector<NetRaw> out;
    auto lines = procutil::splitLines(text);
    // /proc/net/dev has two header lines; data lines look like
    //   "  eth0: 12345 67 0 0 0 0 0 0  8910 11 ..."
    for (const auto& line : lines) {
        auto colon = line.find(':');
        if (colon == std::string::npos) continue;  // skips both header lines
        std::string name = line.substr(0, colon);
        // trim leading/trailing whitespace from the interface name
        std::size_t s = name.find_first_not_of(" \t");
        std::size_t e = name.find_last_not_of(" \t");
        if (s == std::string::npos) continue;
        name = name.substr(s, e - s + 1);
        if (name == "lo") continue;

        auto t = procutil::tokenize(line.substr(colon + 1));
        // rx: bytes packets errs drop fifo frame compressed multicast (8)
        // tx: bytes packets errs drop fifo colls carrier compressed (8)
        if (t.size() < 16) continue;
        NetRaw n;
        n.name = name;
        n.rx_bytes = procutil::toU64(t[0]);
        n.tx_bytes = procutil::toU64(t[8]);
        out.push_back(std::move(n));
    }
    return out;
}

NetSample NetCollector::computeSample(const NetRaw& prev, const NetRaw& cur, double dt) {
    NetSample s;
    s.name = cur.name;
    if (dt <= 0.0) return s;
    auto delta = [](uint64_t a, uint64_t b) -> double {
        return (b >= a) ? static_cast<double>(b - a) : 0.0;
    };
    s.rx_bps = delta(prev.rx_bytes, cur.rx_bytes) / dt;
    s.tx_bps = delta(prev.tx_bytes, cur.tx_bytes) / dt;
    return s;
}

std::vector<NetSample> NetCollector::update(const std::string& procRoot,
                                            const std::string& sysRoot, double dt) {
    std::string text;
    std::vector<NetSample> out;
    if (!procutil::readFile(procutil::join(procRoot, "net/dev"), text)) return out;

    auto cur = parseText(text);
    std::map<std::string, NetRaw> curMap;
    for (const auto& n : cur) curMap[n.name] = n;

    for (const auto& n : cur) {
        NetSample s;
        s.name = n.name;
        if (hasPrev_) {
            auto it = prev_.find(n.name);
            if (it != prev_.end()) s = computeSample(it->second, n, dt);
            s.name = n.name;
        }
        long speed = procutil::readLong(
            procutil::join(sysRoot, "class/net/" + n.name + "/speed"), -1);
        s.speed_mbps = (speed > 0) ? speed : -1;  // sysfs reports -1 for down/virtual links
        out.push_back(std::move(s));
    }

    prev_ = std::move(curMap);
    hasPrev_ = true;
    return out;
}

} // namespace sysperf
