// test_netdev — /proc/net/dev parse (skip lo) + rate delta math.
#include "collector/netdev.h"
#include "test_util.h"

using namespace sysperf;
using systest::fixture;

static void test_parse_skips_lo() {
    auto rows = NetCollector::parseText(
        "Inter-|   Receive                                                |  Transmit\n"
        " face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed\n"
        "    lo: 1000 10 0 0 0 0 0 0 1000 10 0 0 0 0 0 0\n"
        "  eth0: 5000 10 0 0 0 0 0 0 2000 8 0 0 0 0 0 0\n");
    CHECK_EQ(rows.size(), static_cast<std::size_t>(1));  // lo excluded
    CHECK_EQ(rows[0].name, std::string("eth0"));
    CHECK_EQ(rows[0].rx_bytes, 5000ULL);
    CHECK_EQ(rows[0].tx_bytes, 2000ULL);
}

static void test_delta_math() {
    NetRaw prev{"eth0", 5000, 2000};
    NetRaw cur{"eth0", 15000, 6000};
    NetSample s = NetCollector::computeSample(prev, cur, /*dt*/ 2.0);
    CHECK_NEAR(s.rx_bps, 10000.0 / 2.0, 1e-6);  // 5000 B/s
    CHECK_NEAR(s.tx_bps, 4000.0 / 2.0, 1e-6);   // 2000 B/s
}

static void test_stateful_from_fixtures() {
    NetCollector n;
    n.update(fixture("net_t0/proc"), fixture("net_t0/sys"), 0.0);            // prime
    auto out = n.update(fixture("net_t1/proc"), fixture("net_t1/sys"), 2.0);  // dt = 2 s
    bool found = false;
    for (const auto& s : out) {
        if (s.name == "eth0") {
            found = true;
            CHECK_NEAR(s.rx_bps, 5000.0, 1e-6);
            CHECK_NEAR(s.tx_bps, 2000.0, 1e-6);
        }
    }
    CHECK(found);
}

SYSTEST_MAIN({
    test_parse_skips_lo();
    test_delta_math();
    test_stateful_from_fixtures();
})
