#include "collector/proc_util.h"

#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

namespace sysperf::procutil {

bool readFile(const std::string& path, std::string& out) {
    out.clear();
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (!f.is_open()) return false;
    std::ostringstream ss;
    ss << f.rdbuf();
    if (f.bad()) return false;
    out = ss.str();
    return true;
}

std::string readFileOr(const std::string& path, const std::string& fallback) {
    std::string s;
    if (readFile(path, s)) return s;
    return fallback;
}

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> out;
    std::size_t i = 0;
    const std::size_t n = line.size();
    while (i < n) {
        while (i < n && std::isspace(static_cast<unsigned char>(line[i]))) ++i;
        std::size_t start = i;
        while (i < n && !std::isspace(static_cast<unsigned char>(line[i]))) ++i;
        if (i > start) out.emplace_back(line.substr(start, i - start));
    }
    return out;
}

std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> out;
    std::string cur;
    std::istringstream ss(text);
    while (std::getline(ss, cur)) {
        if (!cur.empty() && cur.back() == '\r') cur.pop_back();
        out.push_back(cur);
    }
    return out;
}

std::string join(const std::string& root, const std::string& rel) {
    if (root.empty()) return rel;
    std::string r = root;
    if (r.back() == '/') r.pop_back();
    if (rel.empty()) return r;
    if (rel.front() == '/') return r + rel;
    return r + "/" + rel;
}

bool fileExists(const std::string& path) {
    struct stat st {};
    return ::stat(path.c_str(), &st) == 0;
}

uint64_t toU64(const std::string& s, uint64_t def) {
    if (s.empty()) return def;
    errno = 0;
    char* end = nullptr;
    unsigned long long v = std::strtoull(s.c_str(), &end, 10);
    if (end == s.c_str() || errno != 0) return def;
    return static_cast<uint64_t>(v);
}

long toLong(const std::string& s, long def) {
    if (s.empty()) return def;
    errno = 0;
    char* end = nullptr;
    long v = std::strtol(s.c_str(), &end, 10);
    if (end == s.c_str() || errno != 0) return def;
    return v;
}

double toDouble(const std::string& s, double def) {
    if (s.empty()) return def;
    errno = 0;
    char* end = nullptr;
    double v = std::strtod(s.c_str(), &end);
    if (end == s.c_str() || errno != 0) return def;
    return v;
}

long readLong(const std::string& path, long def) {
    std::string s;
    if (!readFile(path, s)) return def;
    // Find the first integer token (handles leading whitespace / trailing units).
    auto toks = tokenize(s);
    if (toks.empty()) return def;
    return toLong(toks.front(), def);
}

uint64_t readU64(const std::string& path, uint64_t def) {
    std::string s;
    if (!readFile(path, s)) return def;
    auto toks = tokenize(s);
    if (toks.empty()) return def;
    return toU64(toks.front(), def);
}

} // namespace sysperf::procutil
