cmake_minimum_required(VERSION 3.16)

# ── Version comes from version.txt (single source of truth) ────────────────────
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" SYSPERF_VERSION_RAW LIMIT_COUNT 1)
string(STRIP "${SYSPERF_VERSION_RAW}" SYSPERF_VERSION)
# Strip any pre-release/build suffix for project() (it wants pure X.Y.Z)
string(REGEX MATCH "^[0-9]+\\.[0-9]+\\.[0-9]+" SYSPERF_VERSION_CORE "${SYSPERF_VERSION}")

project(sysperf VERSION "${SYSPERF_VERSION_CORE}" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

option(SYSPERF_SANITIZE "Enable ASan + UBSan (debug builds)" OFF)
option(SYSPERF_BUILD_TESTS "Build the fixture/budget tests" ON)
option(SYSPERF_BUILD_GUI "Build the Qt6 GUI (off ⇒ core+tests only, no Qt needed)" ON)

# ── git sha + build timestamp for the About box / release.json parity ──────────
find_package(Git QUIET)
set(SYSPERF_GIT_SHA "nogit")
if(Git_FOUND)
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE SYSPERF_GIT_SHA
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET)
  if(NOT SYSPERF_GIT_SHA)
    set(SYSPERF_GIT_SHA "nogit")
  endif()
endif()
string(TIMESTAMP SYSPERF_BUILT_AT "%Y-%m-%dT%H:%M:%SZ" UTC)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h"
  @ONLY)

# ── Warnings / sanitizers ──────────────────────────────────────────────────────
add_library(sysperf_warnings INTERFACE)
target_compile_options(sysperf_warnings INTERFACE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>)
if(SYSPERF_SANITIZE)
  target_compile_options(sysperf_warnings INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-fsanitize=address,undefined -fno-omit-frame-pointer>)
  target_link_options(sysperf_warnings INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-fsanitize=address,undefined>)
endif()

# ── Core collector library: pure C++17, NO Qt. Fixture-testable. ───────────────
add_library(sysperf_core STATIC
  src/collector/proc_util.cpp
  src/collector/cpu.cpp
  src/collector/meminfo.cpp
  src/collector/diskstats.cpp
  src/collector/netdev.cpp
  src/collector/gpu_nvml.cpp
  src/collector/gpu_fdinfo.cpp
  src/collector/self_metrics.cpp
)
target_include_directories(sysperf_core PUBLIC src)
target_link_libraries(sysperf_core PUBLIC ${CMAKE_DL_LIBS})   # dlopen for NVML
target_link_libraries(sysperf_core PRIVATE sysperf_warnings)

# ── GUI executable: Qt6 Widgets, dynamically linked (LGPLv3). ──────────────────
if(SYSPERF_BUILD_GUI)
  set(CMAKE_AUTOMOC ON)
  find_package(Qt6 REQUIRED COMPONENTS Widgets)

  add_executable(sysperf
    src/main.cpp
    src/config.cpp
    src/cli_output.cpp
    src/sysperf.qrc
    src/collector/worker.cpp
    src/ui/mainwindow.cpp
    src/ui/graphwidget.cpp
    src/ui/minigraph.cpp
    src/ui/corebars.cpp
    src/ui/panel.cpp
    src/ui/format.cpp
  )
  target_link_libraries(sysperf PRIVATE sysperf_core sysperf_warnings Qt6::Widgets)
  target_include_directories(sysperf PRIVATE src)

  install(TARGETS sysperf RUNTIME DESTINATION bin)
  install(FILES packaging/sysperf.desktop DESTINATION share/applications)
  install(FILES packaging/sysperf.svg DESTINATION share/icons/hicolor/scalable/apps)
endif()

# ── Tests ──────────────────────────────────────────────────────────────────────
if(SYSPERF_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
