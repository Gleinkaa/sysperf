#include "ui/format.h"

namespace sysperf::fmt {

QString bytes(uint64_t n) {
    static const char* units[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB"};
    double v = static_cast<double>(n);
    int u = 0;
    while (v >= 1024.0 && u < 5) {
        v /= 1024.0;
        ++u;
    }
    return QString::asprintf(u == 0 ? "%.0f %s" : "%.2f %s", v, units[u]);
}

QString rate(double bytesPerSec) {
    if (bytesPerSec < 1.0) return QStringLiteral("0 B/s");
    static const char* units[] = {"B/s", "KB/s", "MB/s", "GB/s", "TB/s"};
    double v = bytesPerSec;
    int u = 0;
    while (v >= 1000.0 && u < 4) {  // decimal scaling for throughput readouts
        v /= 1000.0;
        ++u;
    }
    return QString::asprintf(u == 0 ? "%.0f %s" : "%.1f %s", v, units[u]);
}

QString bitrate(double bytesPerSec) {
    double bits = bytesPerSec * 8.0;
    if (bits < 1.0) return QStringLiteral("0 bps");
    static const char* units[] = {"bps", "Kbps", "Mbps", "Gbps", "Tbps"};
    double v = bits;
    int u = 0;
    while (v >= 1000.0 && u < 4) {
        v /= 1000.0;
        ++u;
    }
    return QString::asprintf(u == 0 ? "%.0f %s" : "%.1f %s", v, units[u]);
}

QString ghz(double v) {
    if (v <= 0.0) return QStringLiteral("—");
    return QString::asprintf("%.2f GHz", v);
}

QString percent(double v, int decimals) {
    if (v < 0.0) v = 0.0;
    if (v > 100.0) v = 100.0;
    return QString::asprintf("%.*f %%", decimals, v);
}

QString tempC(double v) {
    if (v < 0.0) return QStringLiteral("—");
    return QString::asprintf("%.0f °C", v);
}

QString watts(double v) {
    if (v < 0.0) return QStringLiteral("—");
    return QString::asprintf("%.0f W", v);
}

QString linkSpeed(long mbps) {
    if (mbps <= 0) return QStringLiteral("—");
    if (mbps >= 1000 && (mbps % 1000) == 0)
        return QString::asprintf("%ld Gbps", mbps / 1000);
    return QString::asprintf("%ld Mbps", mbps);
}

} // namespace sysperf::fmt
