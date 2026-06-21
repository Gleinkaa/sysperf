#include "collector/diskstats.h"

#include <cctype>

#include "collector/proc_util.h"

namespace sysperf {

namespace {
bool startsWith(const std::string& s, const char* p) { return s.rfind(p, 0) == 0; }

bool endsWithDigits(const std::string& s) {
    return !s.empty() && std::isdigit(static_cast<unsigned char>(s.back()));
}
} // namespace

bool DiskCollector::isWholeDisk(const std::string& name) {
    if (name.empty()) return false;
    // Virtual / non-physical devices we never want to chart.
    if (startsWith(name, "loop") || startsWith(name, "ram") || startsWith(name, "zram") ||
        startsWith(name, "fd") || startsWith(name, "sr") || startsWith(name, "md")) {
        return false;
    }
    // NVMe / MMC partitions: "<disk>p<N>" e.g. nvme0n1p1, mmcblk0p1.
    if (startsWith(name, "nvme") || startsWith(name, "mmcblk")) {
        auto pPos = name.rfind('p');
        if (pPos != std::string::npos && pPos + 1 < name.size()) {
            bool allDigits = true;
            for (std::size_t i = pPos + 1; i < name.size(); ++i)
                if (!std::isdigit(static_cast<unsigned char>(name[i]))) { allDigits = false; break; }
            if (allDigits) return false;  // it's a partition
        }
        return true;  // whole nvme0n1 / mmcblk0
    }
    // dm-* (LVM/crypt mapped devices) are whole devices.
    if (startsWith(name, "dm-")) return true;
    // sd*/vd*/hd*/xvd*: a whole disk has no trailing digit (sda); sda1 is a partition.
    if (startsWith(name, "sd") || startsWith(name, "vd") || startsWith(name, "hd") ||
        startsWith(name, "xvd")) {
        return !endsWithDigits(name);
    }
    // Unknown naming: include it rather than silently drop a real device.
    return true;
}

std::vector<DiskRaw> DiskCollector::parseText(const std::string& text) {
    std::vector<DiskRaw> out;
    for (const auto& line : procutil::splitLines(text)) {
        auto t = procutil::tokenize(line);
        // major minor name + 11 fields (older kernels) or more. Need at least name+10.
        if (t.size() < 13) continue;
        const std::string& name = t[2];
        if (!isWholeDisk(name)) continue;
        DiskRaw d;
        d.name = name;
        // Fields after name start at t[3]:
        //   t[3]=reads, t[4]=reads_merged, t[5]=sectors_read, t[6]=ms_reading,
        //   t[7]=writes, t[8]=writes_merged, t[9]=sectors_written, t[10]=ms_writing,
        //   t[11]=ios_in_progress, t[12]=io_ticks(ms), ...
        d.sectors_read = procutil::toU64(t[5]);
        d.sectors_written = procutil::toU64(t[9]);
        d.io_ticks_ms = procutil::toU64(t[12]);
        out.push_back(std::move(d));
    }
    return out;
}

DiskSample DiskCollector::computeSample(const DiskRaw& prev, const DiskRaw& cur, double dt) {
    DiskSample s;
    s.name = cur.name;
    if (dt <= 0.0) return s;  // first sample: no delta yet

    auto delta = [](uint64_t a, uint64_t b) -> double {
        return (b >= a) ? static_cast<double>(b - a) : 0.0;  // guard counter resets
    };

    const double ioMs = delta(prev.io_ticks_ms, cur.io_ticks_ms);
    double util = 100.0 * ioMs / (dt * 1000.0);
    if (util < 0.0) util = 0.0;
    if (util > 100.0) util = 100.0;
    s.util_pct = util;

    constexpr double SECTOR = 512.0;
    s.read_bps = delta(prev.sectors_read, cur.sectors_read) * SECTOR / dt;
    s.write_bps = delta(prev.sectors_written, cur.sectors_written) * SECTOR / dt;
    return s;
}

std::vector<DiskSample> DiskCollector::update(const std::string& procRoot,
                                              const std::string& sysRoot, double dt) {
    std::string text;
    std::vector<DiskSample> out;
    if (!procutil::readFile(procutil::join(procRoot, "diskstats"), text)) return out;

    auto cur = parseText(text);
    std::map<std::string, DiskRaw> curMap;
    for (const auto& d : cur) curMap[d.name] = d;

    for (const auto& d : cur) {
        DiskSample s;
        if (hasPrev_) {
            auto it = prev_.find(d.name);
            s = computeSample(it != prev_.end() ? it->second : d, d, hasPrev_ ? dt : 0.0);
            if (it == prev_.end()) s = DiskSample{};  // new device this tick → no delta
            s.name = d.name;
        } else {
            s.name = d.name;
        }
        // rotational: 1 = HDD, 0 = SSD/NVMe. Best-effort from sysfs.
        std::string rotPath = procutil::join(sysRoot, "block/" + d.name + "/queue/rotational");
        long rot = procutil::readLong(rotPath, -1);
        if (rot >= 0) {
            s.has_rotational = true;
            s.rotational = (rot == 1);
        }
        out.push_back(std::move(s));
    }

    prev_ = std::move(curMap);
    hasPrev_ = true;
    return out;
}

} // namespace sysperf
