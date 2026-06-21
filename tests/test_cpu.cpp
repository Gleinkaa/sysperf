// test_cpu — /proc/stat parse + jiffy-delta utilisation math.
#include "collector/cpu.h"
#include "test_util.h"

using namespace sysperf;
using systest::fixture;

static void test_parse_and_util() {
    ParsedStat t0 = CpuCollector::readStat(fixture("cpu_t0/proc"));
    ParsedStat t1 = CpuCollector::readStat(fixture("cpu_t1/proc"));
    CHECK(t0.ok);
    CHECK(t1.ok);
    CHECK_EQ(t0.cores.size(), static_cast<std::size_t>(2));
    CHECK_EQ(t1.cores.size(), static_cast<std::size_t>(2));

    // Aggregate: total delta = 1000, idle delta = 700 ⇒ busy 300 ⇒ 30.0 %.
    double agg = CpuCollector::utilization(t0.aggregate, t1.aggregate);
    CHECK_NEAR(agg, 30.0, 1e-6);

    // Each core moves identically ⇒ 30 % each.
    for (std::size_t i = 0; i < 2; ++i) {
        double u = CpuCollector::utilization(t0.cores[i], t1.cores[i]);
        CHECK_NEAR(u, 30.0, 1e-6);
    }
}

static void test_edges() {
    CpuTimes z{};
    // No progress between identical samples ⇒ 0 %, not NaN.
    CHECK_NEAR(CpuCollector::utilization(z, z), 0.0, 1e-9);

    // Fully busy: all of the delta is non-idle ⇒ 100 %.
    CpuTimes a{};
    CpuTimes b{};
    b.user = 100;  // 100 jiffies of pure user time, no idle
    CHECK_NEAR(CpuCollector::utilization(a, b), 100.0, 1e-9);
}

static void test_model() {
    std::string model = CpuCollector::readModel(fixture("cpu_t0/proc"));
    CHECK(!model.empty());
    CHECK_EQ(model, std::string("Generic CPU @ 0.00GHz"));
}

static void test_stateful_update() {
    CpuCollector c;
    CpuInfo first = c.update(fixture("cpu_t0/proc"), fixture("cpu_t0/sys"));
    CHECK_NEAR(first.total_pct, 0.0, 1e-9);  // first sample: no delta yet
    CHECK_EQ(first.logical_cores, 2);

    CpuInfo second = c.update(fixture("cpu_t1/proc"), fixture("cpu_t1/sys"));
    CHECK_NEAR(second.total_pct, 30.0, 1e-6);
    CHECK_EQ(second.per_core_pct.size(), static_cast<std::size_t>(2));
    CHECK_NEAR(second.per_core_pct[0], 30.0, 1e-6);
}

SYSTEST_MAIN({
    test_parse_and_util();
    test_edges();
    test_model();
    test_stateful_update();
})
