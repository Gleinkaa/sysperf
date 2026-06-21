// format.h — human-readable formatting for byte counts, throughput, frequency.
#pragma once

#include <QString>
#include <cstdint>

namespace sysperf::fmt {

// 1536 → "1.5 KiB", 2'000'000'000 → "1.86 GiB".
QString bytes(uint64_t n);

// Bytes/sec → "12.3 MB/s" (decimal, like file managers) or "0 B/s".
QString rate(double bytesPerSec);

// Bytes/sec → bits-based "98.5 Mbps" (for network link-style readouts).
QString bitrate(double bytesPerSec);

// 3.6 → "3.60 GHz"; 0 → "—".
QString ghz(double v);

// 42.0 → "42 %"; clamps display to 0..100.
QString percent(double v, int decimals = 0);

// 65.0 → "65 °C"; <0 → "—".
QString tempC(double v);

// 120.5 → "120 W"; <0 → "—".
QString watts(double v);

// link speed Mbps (-1 → "—", 1000 → "1 Gbps").
QString linkSpeed(long mbps);

} // namespace sysperf::fmt
