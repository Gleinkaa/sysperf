// test_util.h — a deliberately tiny test harness (no GoogleTest dependency, to keep
// the build to just build-essential + cmake + qt6-base-dev + libdrm-dev). Each test
// is its own executable that returns non-zero on failure; CTest reports pass/fail.
#pragma once

#include <cmath>
#include <cstdio>
#include <string>

#ifndef SYSPERF_FIXTURES_DIR
#define SYSPERF_FIXTURES_DIR "."
#endif

namespace systest {
inline int g_failures = 0;

// Absolute path to a fixture, e.g. fixture("cpu_t0/proc") → ".../tests/fixtures/cpu_t0/proc".
inline std::string fixture(const std::string& rel) {
    return std::string(SYSPERF_FIXTURES_DIR) + "/" + rel;
}
} // namespace systest

#define CHECK(cond)                                                                  \
    do {                                                                             \
        if (!(cond)) {                                                               \
            std::fprintf(stderr, "  FAIL %s:%d: CHECK(%s)\n", __FILE__, __LINE__, #cond); \
            ++systest::g_failures;                                                    \
        }                                                                            \
    } while (0)

#define CHECK_EQ(a, b)                                                               \
    do {                                                                             \
        auto _va = (a);                                                              \
        auto _vb = (b);                                                              \
        if (!(_va == _vb)) {                                                         \
            std::fprintf(stderr, "  FAIL %s:%d: %s == %s\n", __FILE__, __LINE__, #a, #b); \
            ++systest::g_failures;                                                    \
        }                                                                            \
    } while (0)

#define CHECK_NEAR(a, b, eps)                                                        \
    do {                                                                             \
        double _da = (a);                                                            \
        double _db = (b);                                                            \
        if (std::fabs(_da - _db) > (eps)) {                                          \
            std::fprintf(stderr, "  FAIL %s:%d: %s ≈ %s  (%.6f vs %.6f, eps %.6f)\n", \
                         __FILE__, __LINE__, #a, #b, _da, _db, (double)(eps));        \
            ++systest::g_failures;                                                    \
        }                                                                            \
    } while (0)

#define SYSTEST_MAIN(body)                                                           \
    int main() {                                                                     \
        body;                                                                        \
        if (systest::g_failures) {                                                   \
            std::fprintf(stderr, "%d check(s) failed\n", systest::g_failures);       \
            return 1;                                                                \
        }                                                                            \
        std::printf("ok\n");                                                         \
        return 0;                                                                    \
    }
