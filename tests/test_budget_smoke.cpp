// test_budget_smoke — integration smoke test. Drives the full stateful collector
// pipeline (two ticks) against fixtures, assembles a Snapshot the way the worker
// does, and asserts the composed result is plausible AND that the app's own
// self-metric stays inside the soft budget. This is the CTest stand-in for the
// "run 5 min under load and check PSS/CPU" guardrail (which needs a live system).
#include "collector/cpu.h"
#include "collector/diskstats.h"
#include "collector/meminfo.h"
#include "collector/netdev.h"
#include "collector/self_metrics.h"
#include "model/snapshot.h"
#include "test_util.h"

using namespace sysperf;
using systest::fixture;

static Snapshot tick(CpuCollector& cpu, MemCollector& mem, DiskCollector& disk,
                     NetCollector& net, SelfCollector& self, const std::string& t,
                     double dt, bool first) {
    Snapshot s;
    s.first = first;
    s.dt = dt;
    s.cpu = cpu.update(fixture(t + "/proc"), fixture(t + "/sys"));
    s.mem = mem.update(fixture(t + "/proc"));
    s.disks = disk.update(fixture(t + "/proc"), fixture(t + "/sys"), dt);
    s.nets = net.update(fixture(t + "/proc"), fixture(t + "/sys"), dt);
    BudgetLimits lim;
    s.self = self.update(fixture(t + "/proc"), dt, lim, 100);
    return s;
}

int main() {
    CpuCollector cpu;
    MemCollector mem;
    DiskCollector disk;
    NetCollector net;
    SelfCollector self;

    // Fixture tree "smoke_t0" and "smoke_t1" each carry a full /proc + /sys snapshot.
    Snapshot s0 = tick(cpu, mem, disk, net, self, "smoke_t0", 0.0, true);
    Snapshot s1 = tick(cpu, mem, disk, net, self, "smoke_t1", 1.0, false);

    // First tick has no deltas; second tick should produce the expected values.
    CHECK_NEAR(s0.cpu.total_pct, 0.0, 1e-9);
    CHECK_NEAR(s1.cpu.total_pct, 30.0, 1e-6);

    // Memory is stateless: honest "in use" both ticks.
    CHECK(s1.mem.total > 0);
    CHECK_EQ(s1.mem.used, s1.mem.total - s1.mem.available);

    // At least one whole disk and one non-loopback NIC with sane derived rates.
    CHECK(!s1.disks.empty());
    CHECK(!s1.nets.empty());
    for (const auto& d : s1.disks) {
        CHECK(d.util_pct >= 0.0 && d.util_pct <= 100.0);
        CHECK(d.read_bps >= 0.0 && d.write_bps >= 0.0);
    }
    for (const auto& n : s1.nets) {
        CHECK(n.rx_bps >= 0.0 && n.tx_bps >= 0.0);
    }

    // The budget guardrail itself: the (tiny, fixture) PSS is well under the alarm.
    CHECK(!s1.self.over_budget);
    CHECK(s1.self.pss_bytes > 0);

    if (systest::g_failures) {
        std::fprintf(stderr, "%d check(s) failed\n", systest::g_failures);
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
