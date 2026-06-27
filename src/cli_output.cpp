#include "cli_output.h"

#include <QString>

#include <algorithm>
#include <cstdio>
#include <string>
#include <string_view>

#include "ui/format.h"
#include "version.h"

namespace sysperf::cli {

namespace {

// ---- tiny helpers -----------------------------------------------------------

std::string padRight(const std::string& s, int width) {
    if (static_cast<int>(s.size()) >= width) return s;
    return s + std::string(static_cast<std::size_t>(width - s.size()), ' ');
}

QString pct(double v) { return fmt::percent(v, 1); }
QString b(uint64_t n)  { return fmt::bytes(n); }
QString r(double v)    { return fmt::rate(v); }

constexpr std::string_view HLINE = u8"─"; // ─
constexpr std::string_view THICK_HLINE = u8"═"; // ═

// ---- sections ---------------------------------------------------------------

std::string renderHeader() {
    std::string out;
    out += "══ sysperf " SYSPERF_VERSION;
    for (int i = static_cast<int>(out.size()); i < 64; ++i) out += THICK_HLINE;
    out += '\n';
    return out;
}

std::string renderCpu(const CpuInfo& c) {
    std::string out;
    out += "\n── CPU ";
    for (int i = 6; i < 64; ++i) out += HLINE;
    out += '\n';

    out += "  Total:  " + padRight(pct(c.total_pct).toStdString(), 14);
    out += "Speed:  " + padRight(fmt::ghz(c.freq_ghz).toStdString(), 16);
    out += "Cores:  " + std::to_string(c.logical_cores) + " logical\n";

    out += "  Busiest core:  " + fmt::percent(*std::max_element(c.per_core_pct.begin(),
                                                         c.per_core_pct.end()), 0).toStdString() + '\n';
    if (!c.model.empty())
        out += "  Model:  " + c.model + '\n';
    return out;
}

std::string renderMem(const MemInfo& m) {
    std::string out;
    out += "\n── Memory ";
    for (int i = 10; i < 64; ++i) out += HLINE;
    out += '\n';

    double usedPct = (m.total > 0) ? 100.0 * static_cast<double>(m.used)
                                          / static_cast<double>(m.total)
                                   : 0.0;
    out += "  Used:   " + padRight(b(m.used).toStdString(), 14);
    out += "/  " + padRight(b(m.total).toStdString(), 14);
    out += "(" + pct(usedPct).toStdString() + ")\n";

    out += "  Avail:  " + padRight(b(m.available).toStdString(), 14);
    out += "Cached:  " + b(m.cached).toStdString() + '\n';

    uint64_t swapUsed = (m.swap_total > m.swap_free) ? (m.swap_total - m.swap_free) : 0ULL;
    out += "  Swap:   " + padRight(b(swapUsed).toStdString(), 14);
    out += "/  " + b(m.swap_total).toStdString() + '\n';

    out += "  Committed:  " + b(m.committed).toStdString();
    out += " / " + b(m.commit_limit).toStdString() + '\n';
    return out;
}

std::string renderDisks(const std::vector<DiskSample>& disks) {
    if (disks.empty()) return {};
    std::string out;
    out += "\n── Disk ";
    for (int i = 8; i < 64; ++i) out += HLINE;
    out += '\n';

    for (const auto& d : disks) {
        out += "  " + padRight(d.name, 16);
        out += padRight(pct(d.util_pct).toStdString(), 8);
        out += "active  │";
        out += "  R " + padRight(r(d.read_bps).toStdString(), 13);
        out += "│  W " + r(d.write_bps).toStdString();
        out += "  [" + std::string(d.has_rotational ? (d.rotational ? "HDD" : "SSD") : "?") + "]\n";
    }
    return out;
}

std::string renderNets(const std::vector<NetSample>& nets) {
    if (nets.empty()) return {};
    std::string out;
    out += "\n── Network ";
    for (int i = 11; i < 64; ++i) out += HLINE;
    out += '\n';

    for (const auto& n : nets) {
        out += "  " + padRight(n.name, 12);
        out += "R " + padRight(fmt::bitrate(n.rx_bps).toStdString(), 14);
        out += "│  S " + padRight(fmt::bitrate(n.tx_bps).toStdString(), 14);
        out += "│  Link " + fmt::linkSpeed(n.speed_mbps).toStdString() + '\n';
    }
    return out;
}

std::string renderGpus(const std::vector<GpuSample>& gpus) {
    if (gpus.empty()) return {};
    std::string out;
    out += "\n── GPU ";
    for (int i = 7; i < 64; ++i) out += HLINE;
    out += '\n';

    for (const auto& g : gpus) {
        out += "  " + padRight(g.name, 20);
        out += "Util:  " + padRight(g.util_pct >= 0.0 ? pct(g.util_pct).toStdString() : "—", 10);
        if (g.mem_total > 0)
            out += "Mem:  " + b(g.mem_used).toStdString() + " / " + b(g.mem_total).toStdString();
        out += '\n';
        out += std::string(22, ' ');
        out += "Temp:  " + padRight(fmt::tempC(g.temp_c).toStdString(), 10);
        out += "Clock: " + padRight(g.clock_mhz >= 0.0
                                    ? (std::to_string(static_cast<int>(g.clock_mhz)) + " MHz")
                                    : "—", 12);
        out += "Power: " + fmt::watts(g.power_w).toStdString() + '\n';
    }
    return out;
}

std::string renderSelf(const SelfMetrics& m) {
    std::string out;
    out += "\n── Self ";
    for (int i = 8; i < 64; ++i) out += HLINE;
    out += '\n';

    out += "  PSS:  " + padRight(b(m.pss_bytes).toStdString(), 14);
    out += "CPU:  " + pct(m.cpu_pct).toStdString();
    if (m.over_budget) out += "  ⚠ OVER BUDGET";
    out += '\n';
    return out;
}

} // namespace

std::string render(const Snapshot& s) {
    // Skip the very first sample — it has no deltas yet.
    if (s.first) return {};

    std::string out = renderHeader();
    out += renderCpu(s.cpu);
    out += renderMem(s.mem);
    out += renderDisks(s.disks);
    out += renderNets(s.nets);
    out += renderGpus(s.gpus);
    out += renderSelf(s.self);
    return out;
}

} // namespace sysperf::cli
