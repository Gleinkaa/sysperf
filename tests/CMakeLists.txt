# Fixture-based collector unit tests + budget smoke test. All headless (no Qt, no
# display) — they link only sysperf_core.

set(SYSPERF_FIXTURES "${CMAKE_CURRENT_SOURCE_DIR}/fixtures")

function(add_sysperf_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} PRIVATE sysperf_core sysperf_warnings)
  target_include_directories(${name} PRIVATE
    "${CMAKE_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}")
  target_compile_definitions(${name} PRIVATE SYSPERF_FIXTURES_DIR="${SYSPERF_FIXTURES}")
  add_test(NAME ${name} COMMAND ${name})
endfunction()

add_sysperf_test(test_cpu)
add_sysperf_test(test_meminfo)
add_sysperf_test(test_diskstats)
add_sysperf_test(test_netdev)
add_sysperf_test(test_fdinfo)
add_sysperf_test(test_self)
add_sysperf_test(test_budget_smoke)
