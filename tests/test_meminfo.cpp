// test_meminfo — honest Windows→Linux mapping from /proc/meminfo.
#include "collector/meminfo.h"
#include "test_util.h"

using namespace sysperf;
using systest::fixture;

static constexpr uint64_t KB = 1024;

static void test_mapping() {
    MemCollector m;
    MemInfo info = m.update(fixture("mem/proc"));

    // From the fixture (kB): MemTotal 16000000, MemAvailable 8000000, MemFree 2000000,
    // Buffers 500000, Cached 4000000, SReclaimable 600000, SUnreclaim 300000,
    // Committed_AS 9000000, CommitLimit 10000000, SwapTotal 2000000, SwapFree 1500000.
    CHECK_EQ(info.total, 16000000ULL * KB);
    CHECK_EQ(info.available, 8000000ULL * KB);

    // In use = MemTotal − MemAvailable  (NOT − MemFree).
    CHECK_EQ(info.used, (16000000ULL - 8000000ULL) * KB);
    // Cached = Cached + Buffers + SReclaimable.
    CHECK_EQ(info.cached, (4000000ULL + 500000ULL + 600000ULL) * KB);

    // Slab counters relabelled honestly (not faked as Windows pools).
    CHECK_EQ(info.s_reclaimable, 600000ULL * KB);
    CHECK_EQ(info.s_unreclaim, 300000ULL * KB);

    CHECK_EQ(info.committed, 9000000ULL * KB);
    CHECK_EQ(info.commit_limit, 10000000ULL * KB);
    CHECK_EQ(info.swap_total, 2000000ULL * KB);
    CHECK_EQ(info.swap_free, 1500000ULL * KB);

    // The trap: total − free would be wildly larger than the honest "in use".
    uint64_t naive = (info.total - info.free);
    CHECK(naive > info.used);
}

SYSTEST_MAIN({ test_mapping(); })
