// test_diskstats — whole-disk filtering + io_ticks/throughput delta math.
#include "collector/diskstats.h"
#include "test_util.h"

using namespace sysperf;
using systest::fixture;

static void test_whole_disk_filter() {
    CHECK(DiskCollector::isWholeDisk("sda"));
    CHECK(!DiskCollector::isWholeDisk("sda1"));
    CHECK(DiskCollector::isWholeDisk("nvme0n1"));
    CHECK(!DiskCollector::isWholeDisk("nvme0n1p1"));
    CHECK(DiskCollector::isWholeDisk("mmcblk0"));
    CHECK(!DiskCollector::isWholeDisk("mmcblk0p1"));
    CHECK(DiskCollector::isWholeDisk("dm-0"));
    CHECK(!DiskCollector::isWholeDisk("loop0"));
    CHECK(!DiskCollector::isWholeDisk("ram3"));
    CHECK(!DiskCollector::isWholeDisk("zram0"));
}

static void test_parse_filters_partitions() {
    std::string body;
    // Reuse the fixture file content via the collector's read path.
    auto rows = DiskCollector::parseText(
        "   8       0 sda 110 0 2200 55 95 0 4500 45 0 2000 95\n"
        "   8       1 sda1 5 0 10 1 2 0 8 1 0 3 1\n"
        " 259       0 nvme0n1 1 0 8 1 1 0 8 1 0 9 1\n"
        "   7       0 loop0 1 0 2 0 0 0 0 0 0 1 0\n");
    // sda + nvme0n1 kept; sda1 + loop0 dropped.
    CHECK_EQ(rows.size(), static_cast<std::size_t>(2));
    bool sawSda = false, sawNvme = false;
    for (const auto& r : rows) {
        if (r.name == "sda") sawSda = true;
        if (r.name == "nvme0n1") sawNvme = true;
    }
    CHECK(sawSda);
    CHECK(sawNvme);
}

static void test_delta_math() {
    DiskRaw prev{"sda", /*sr*/ 2000, /*sw*/ 4000, /*io_ms*/ 1500};
    DiskRaw cur{"sda", /*sr*/ 2200, /*sw*/ 4500, /*io_ms*/ 2000};
    DiskSample s = DiskCollector::computeSample(prev, cur, /*dt*/ 1.0);

    // io_ticks delta 500 ms over 1000 ms ⇒ 50 % active.
    CHECK_NEAR(s.util_pct, 50.0, 1e-6);
    // sectors_read delta 200 * 512 / 1 s.
    CHECK_NEAR(s.read_bps, 200.0 * 512.0, 1e-6);
    CHECK_NEAR(s.write_bps, 500.0 * 512.0, 1e-6);

    // Half-second interval doubles the rates and the %active.
    DiskSample s2 = DiskCollector::computeSample(prev, cur, 0.5);
    CHECK_NEAR(s2.util_pct, 100.0, 1e-6);  // 500 ms busy in a 500 ms window ⇒ clamp 100
    CHECK_NEAR(s2.read_bps, 200.0 * 512.0 / 0.5, 1e-6);
}

static void test_stateful_from_fixtures() {
    DiskCollector d;
    d.update(fixture("disk_t0/proc"), fixture("disk_t0/sys"), 0.0);            // prime
    auto out = d.update(fixture("disk_t1/proc"), fixture("disk_t1/sys"), 1.0);  // dt = 1 s
    CHECK(!out.empty());
    bool found = false;
    for (const auto& s : out) {
        if (s.name == "sda") {
            found = true;
            CHECK_NEAR(s.util_pct, 50.0, 1e-6);
            CHECK_NEAR(s.read_bps, 200.0 * 512.0, 1e-6);
        }
    }
    CHECK(found);
}

SYSTEST_MAIN({
    test_whole_disk_filter();
    test_parse_filters_partitions();
    test_delta_math();
    test_stateful_from_fixtures();
})
