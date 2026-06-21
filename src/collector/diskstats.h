// diskstats.h — per-disk %active and throughput from /proc/diskstats.
//   %active  = io_ticks delta (ms) / measured dt (ms), clamped 0..100
//   read/s   = (sectors_read  delta) * 512 / dt
//   write/s  = (sectors_write delta) * 512 / dt
// Sector size is fixed at 512 B in /proc/diskstats regardless of physical sector size.
#pragma once

#include <map>
#include <string>
#include <vector>

#include "model/snapshot.h"

namespace sysperf {

// Raw counters for one block device line.
struct DiskRaw {
    std::string name;
    uint64_t sectors_read = 0;   // field 3 after name
    uint64_t sectors_written = 0;// field 7 after name
    uint64_t io_ticks_ms = 0;    // field 10 after name ("time spent doing I/Os")
};

class DiskCollector {
public:
    // Parse a /proc/diskstats body. Includes only whole disks (partitions, loop,
    // ram, zram, sr, fd devices are filtered out by isWholeDisk()).
    static std::vector<DiskRaw> parseText(const std::string& text);

    // Heuristic whole-disk filter that works without sysfs (so fixtures are
    // deterministic). sda → true, sda1 → false, nvme0n1 → true, nvme0n1p1 → false,
    // loop0/ram0/zram0/sr0/fd0 → false, dm-0 → true.
    static bool isWholeDisk(const std::string& name);

    // Derive one DiskSample from two raw samples and the measured dt (seconds).
    static DiskSample computeSample(const DiskRaw& prev, const DiskRaw& cur, double dt);

    // Stateful update: reads procRoot, diffs against stored previous, returns one
    // sample per whole disk. rotational is filled from sysRoot when available.
    std::vector<DiskSample> update(const std::string& procRoot, const std::string& sysRoot,
                                   double dt);

private:
    bool hasPrev_ = false;
    std::map<std::string, DiskRaw> prev_;
};

} // namespace sysperf
