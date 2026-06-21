// netdev.h — per-interface send/receive throughput from /proc/net/dev.
//   rx/s = (rx_bytes delta) / measured dt
//   tx/s = (tx_bytes delta) / measured dt
// Link speed (Mbps) comes from /sys/class/net/<if>/speed when available. The
// loopback interface is excluded.
#pragma once

#include <map>
#include <string>
#include <vector>

#include "model/snapshot.h"

namespace sysperf {

struct NetRaw {
    std::string name;
    uint64_t rx_bytes = 0;
    uint64_t tx_bytes = 0;
};

class NetCollector {
public:
    // Parse a /proc/net/dev body. Skips the two header lines and the "lo" device.
    static std::vector<NetRaw> parseText(const std::string& text);

    static NetSample computeSample(const NetRaw& prev, const NetRaw& cur, double dt);

    std::vector<NetSample> update(const std::string& procRoot, const std::string& sysRoot,
                                  double dt);

private:
    bool hasPrev_ = false;
    std::map<std::string, NetRaw> prev_;
};

} // namespace sysperf
