// meminfo.h — memory figures from /proc/meminfo with HONEST Windows→Linux mapping.
//   In use   = MemTotal − MemAvailable   (never MemTotal − MemFree)
//   Cached   = Cached + Buffers + SReclaimable
//   "pool"   = SReclaimable / SUnreclaim slab (relabelled honestly, not faked)
#pragma once

#include <map>
#include <string>

#include "model/snapshot.h"

namespace sysperf {

class MemCollector {
public:
    // Parse a /proc/meminfo body into key → value-in-kB (the file's native unit).
    static std::map<std::string, uint64_t> parseText(const std::string& text);

    // Build the honest MemInfo (byte counts) from a parsed key→kB map.
    static MemInfo fromMap(const std::map<std::string, uint64_t>& kv);

    // Read + parse /proc/meminfo under procRoot. Stateless (no deltas needed).
    MemInfo update(const std::string& procRoot);
};

} // namespace sysperf
