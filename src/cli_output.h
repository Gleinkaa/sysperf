// cli_output.h — terminal-friendly text formatting for --no-gui mode.
// Builds clean, readable sections with spacing from a Snapshot.
#pragma once

#include <string>

#include "model/snapshot_qt.h"

namespace sysperf::cli {

// Render a full snapshot as a multi-line terminal string.
// Sections: CPU, Memory, Disk(s), Network(s), GPU(s), Self.
// Returns an empty string on the first (delta-less) snapshot.
std::string render(const Snapshot& s);

} // namespace sysperf::cli
