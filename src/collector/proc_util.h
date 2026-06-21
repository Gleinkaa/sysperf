// proc_util.h — tiny, dependency-free helpers for reading /proc & /sys. Every
// collector takes a "root" path so fixtures can stand in for the live tree.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace sysperf::procutil {

// Read an entire (small, virtual) file. Returns false and leaves `out` empty on error.
bool readFile(const std::string& path, std::string& out);

// Convenience: returns "" on error.
std::string readFileOr(const std::string& path, const std::string& fallback = "");

// Whitespace-split a single line into tokens (collapses runs of spaces/tabs).
std::vector<std::string> tokenize(const std::string& line);

// Split text into lines (no trailing empty line for a final newline).
std::vector<std::string> splitLines(const std::string& text);

// Join a root with a relative path, inserting exactly one '/'.
std::string join(const std::string& root, const std::string& rel);

bool fileExists(const std::string& path);

// First integer found in a file (e.g. sysfs scalar). Returns `def` on any failure.
long readLong(const std::string& path, long def);
uint64_t readU64(const std::string& path, uint64_t def);

// Parse helpers that never throw.
uint64_t toU64(const std::string& s, uint64_t def = 0);
long toLong(const std::string& s, long def = 0);
double toDouble(const std::string& s, double def = 0.0);

} // namespace sysperf::procutil
