// test_fdinfo — DRM fdinfo parse + engine-busy delta utilisation.
#include "collector/gpu_fdinfo.h"
#include "test_util.h"

using namespace sysperf;

static void test_parse() {
    const std::string body =
        "pos:\t0\n"
        "flags:\t02000002\n"
        "drm-driver:\tamdgpu\n"
        "drm-client-id:\t42\n"
        "drm-pdev:\t0000:03:00.0\n"
        "drm-engine-gfx:\t1000000 ns\n"
        "drm-engine-compute:\t500000 ns\n"
        "drm-memory-vram:\t123 KiB\n";
    FdinfoClient c = FdinfoGpu::parseFdinfo(body);
    CHECK(c.is_drm);
    CHECK_EQ(c.driver, std::string("amdgpu"));
    CHECK_EQ(c.pdev, std::string("0000:03:00.0"));
    CHECK(c.has_client_id);
    CHECK_EQ(c.client_id, 42ULL);
    CHECK_EQ(c.engines.size(), static_cast<std::size_t>(2));
    CHECK_EQ(c.engines.at("gfx"), 1000000ULL);
    CHECK_EQ(c.engines.at("compute"), 500000ULL);
}

static void test_non_drm() {
    FdinfoClient c = FdinfoGpu::parseFdinfo("pos:\t0\nflags:\t0100000\nmnt_id:\t13\n");
    CHECK(!c.is_drm);
    CHECK(c.engines.empty());
}

static void test_engine_util() {
    // 0.5e9 ns of busy time in a 1.0 s window ⇒ 50 %.
    CHECK_NEAR(FdinfoGpu::engineUtil(0, 500000000ULL, 1.0), 50.0, 1e-6);
    // Full second of busy time ⇒ 100 %.
    CHECK_NEAR(FdinfoGpu::engineUtil(0, 1000000000ULL, 1.0), 100.0, 1e-6);
    // Beyond a second (sampling jitter) clamps at 100, never above.
    CHECK_NEAR(FdinfoGpu::engineUtil(0, 1500000000ULL, 1.0), 100.0, 1e-6);
    // Counter reset (cur < prev) ⇒ 0, not negative.
    CHECK_NEAR(FdinfoGpu::engineUtil(1000, 500, 1.0), 0.0, 1e-9);
    // dt = 0 (first sample) ⇒ 0.
    CHECK_NEAR(FdinfoGpu::engineUtil(0, 1000, 0.0), 0.0, 1e-9);
}

SYSTEST_MAIN({
    test_parse();
    test_non_drm();
    test_engine_util();
})
