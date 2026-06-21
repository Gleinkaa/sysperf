// test_self — self-instrumentation parse + soft-budget alarm logic.
#include "collector/self_metrics.h"
#include "test_util.h"

using namespace sysperf;
using systest::fixture;

static void test_parse_pss() {
    const std::string rollup =
        "55a0c0000000-7ffd00000000 ---p 00000000 00:00 0  [rollup]\n"
        "Rss:               40960 kB\n"
        "Pss:               12345 kB\n"
        "Shared_Clean:      20000 kB\n";
    CHECK_EQ(SelfCollector::parsePssKb(rollup), 12345ULL);
}

static void test_parse_cpu_ticks() {
    // Minimal /proc/self/stat: field 14 (utime)=300, field 15 (stime)=120. The comm
    // field deliberately contains spaces and a ')' to exercise the rfind(')') anchor.
    const std::string stat =
        "1234 (weird )name) S 1 1234 1234 0 -1 4194304 100 0 0 0 300 120 0 0 20 0 1 0 1000 5000 50\n";
    CHECK_EQ(SelfCollector::parseCpuTicks(stat), 420ULL);  // 300 + 120
}

static void test_budget_alarm() {
    BudgetLimits lim;  // 50 MB / 1 %
    SelfCollector under;
    SelfMetrics mu = under.update(fixture("self_small/proc"), /*dt*/ 0.0, lim, /*clk*/ 100);
    CHECK_EQ(mu.pss_bytes, 12345ULL * 1024ULL);
    CHECK(!mu.over_budget);  // ~12 MB, well under 50 MB; first tick ⇒ cpu 0

    SelfCollector over;
    SelfMetrics mo = over.update(fixture("self_big/proc"), /*dt*/ 0.0, lim, /*clk*/ 100);
    CHECK(mo.pss_bytes > 50ULL * 1024 * 1024);
    CHECK(mo.over_budget);  // ~78 MB > 50 MB ⇒ alarm trips
}

SYSTEST_MAIN({
    test_parse_pss();
    test_parse_cpu_ticks();
    test_budget_alarm();
})
