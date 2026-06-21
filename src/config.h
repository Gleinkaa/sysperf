// config.h — central settings for the app. Resolution order (later wins):
//   compiled defaults → config/default.json → .env / env vars → CLI flags.
// See config/README.md. Kept Qt-free in its surface so collectors can read it.
#pragma once

#include <string>

class QStringList;

namespace sysperf {

struct AppConfig {
    std::string proc_root = "/proc";
    std::string sys_root = "/sys";
    int interval_ms = 1000;
    int history_seconds = 60;
    double pss_mb_warn = 50.0;
    double cpu_pct_warn = 1.0;
    std::string start_panel = "cpu";
    std::string log_level = "info";

    // Build the effective config from the app's command-line arguments + env +
    // config file. Never throws; bad files fall back to defaults with a warning.
    static AppConfig load(const QStringList& args);
};

} // namespace sysperf
