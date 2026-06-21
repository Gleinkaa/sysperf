#include "collector/gpu_fdinfo.h"

#include <dirent.h>
#include <unistd.h>

#include <algorithm>
#include <cctype>

#include "collector/proc_util.h"

namespace sysperf {

namespace {

std::string trim(const std::string& s) {
    std::size_t a = s.find_first_not_of(" \t");
    if (a == std::string::npos) return {};
    std::size_t b = s.find_last_not_of(" \t");
    return s.substr(a, b - a + 1);
}

bool isAllDigits(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s)
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

GpuVendor vendorFromDriver(const std::string& drv) {
    if (drv == "amdgpu" || drv == "radeon") return GpuVendor::Amd;
    if (drv == "i915" || drv == "xe") return GpuVendor::Intel;
    if (drv == "nvidia" || drv == "nouveau") return GpuVendor::Nvidia;
    return GpuVendor::Unknown;
}

// Best-effort: map a DRM pdev (PCI addr) to a hwmon temp by walking
// /sys/class/drm/card*/device/hwmon/hwmon*/temp1_input (millidegrees C).
double readDeviceTempC(const std::string& sysRoot, const std::string& pdev) {
    DIR* drm = ::opendir(procutil::join(sysRoot, "class/drm").c_str());
    if (!drm) return -1.0;
    double tempC = -1.0;
    struct dirent* e = nullptr;
    while ((e = ::readdir(drm)) != nullptr) {
        std::string card = e->d_name;
        if (card.rfind("card", 0) != 0) continue;
        if (card.find('-') != std::string::npos) continue;  // skip connector nodes card0-eDP-1
        std::string devLink = procutil::join(sysRoot, "class/drm/" + card + "/device");
        char buf[1024] = {0};
        ssize_t n = ::readlink(devLink.c_str(), buf, sizeof(buf) - 1);
        if (n <= 0) continue;
        std::string target(buf, static_cast<std::size_t>(n));
        std::string base = target;
        auto slash = base.find_last_of('/');
        if (slash != std::string::npos) base = base.substr(slash + 1);
        if (base != pdev) continue;

        std::string hwmonDir = devLink + "/hwmon";
        DIR* hw = ::opendir(hwmonDir.c_str());
        if (!hw) break;
        struct dirent* h = nullptr;
        while ((h = ::readdir(hw)) != nullptr) {
            std::string hn = h->d_name;
            if (hn.rfind("hwmon", 0) != 0) continue;
            long milli = procutil::readLong(hwmonDir + "/" + hn + "/temp1_input", -1);
            if (milli > 0) { tempC = milli / 1000.0; break; }
        }
        ::closedir(hw);
        break;
    }
    ::closedir(drm);
    return tempC;
}

} // namespace

FdinfoClient FdinfoGpu::parseFdinfo(const std::string& text) {
    FdinfoClient c;
    for (const auto& line : procutil::splitLines(text)) {
        auto colon = line.find(':');
        if (colon == std::string::npos) continue;
        std::string key = trim(line.substr(0, colon));
        std::string val = trim(line.substr(colon + 1));
        if (key == "drm-driver") {
            c.is_drm = true;
            c.driver = val;
        } else if (key == "drm-pdev") {
            c.is_drm = true;
            c.pdev = val;
        } else if (key == "drm-client-id") {
            c.is_drm = true;
            c.client_id = procutil::toU64(val);
            c.has_client_id = true;
        } else if (key.rfind("drm-engine-", 0) == 0) {
            c.is_drm = true;
            std::string engine = key.substr(std::string("drm-engine-").size());
            // value looks like "12345678 ns" — first token is the count.
            auto toks = procutil::tokenize(val);
            if (!toks.empty()) c.engines[engine] = procutil::toU64(toks[0]);
        }
    }
    return c;
}

double FdinfoGpu::engineUtil(uint64_t prevNs, uint64_t curNs, double dt) {
    if (dt <= 0.0) return 0.0;
    if (curNs <= prevNs) return 0.0;
    double busyNs = static_cast<double>(curNs - prevNs);
    double pct = 100.0 * busyNs / (dt * 1.0e9);
    if (pct < 0.0) pct = 0.0;
    if (pct > 100.0) pct = 100.0;
    return pct;
}

std::map<std::string, DrmDeviceTotals> FdinfoGpu::scan(const std::string& procRoot) {
    std::map<std::string, DrmDeviceTotals> totals;
    // Dedupe shared drm fds per device: drm-client-id is unique within a device,
    // not necessarily across devices, so key the seen-set by "<pdev>/<client-id>".
    std::set<std::string> seenClients;

    DIR* proc = ::opendir(procRoot.c_str());
    if (!proc) return totals;

    struct dirent* pe = nullptr;
    while ((pe = ::readdir(proc)) != nullptr) {
        std::string pid = pe->d_name;
        if (!isAllDigits(pid)) continue;
        std::string fdinfoDir = procutil::join(procRoot, pid + "/fdinfo");
        DIR* fdd = ::opendir(fdinfoDir.c_str());
        if (!fdd) continue;  // gone / permission denied → skip

        struct dirent* fe = nullptr;
        while ((fe = ::readdir(fdd)) != nullptr) {
            std::string fd = fe->d_name;
            if (!isAllDigits(fd)) continue;
            std::string body;
            if (!procutil::readFile(fdinfoDir + "/" + fd, body)) continue;
            if (body.find("drm-") == std::string::npos) continue;  // cheap pre-filter

            FdinfoClient c = parseFdinfo(body);
            if (!c.is_drm || c.pdev.empty() || c.engines.empty()) continue;
            if (c.has_client_id) {
                std::string key = c.pdev + "/" + std::to_string(c.client_id);
                if (seenClients.count(key)) continue;  // already counted
                seenClients.insert(key);
            }
            DrmDeviceTotals& t = totals[c.pdev];
            if (t.driver.empty()) t.driver = c.driver;
            for (const auto& [eng, ns] : c.engines) t.engines[eng] += ns;
        }
        ::closedir(fdd);
    }
    ::closedir(proc);
    return totals;
}

std::vector<GpuSample> FdinfoGpu::update(const std::string& procRoot, const std::string& sysRoot,
                                         double dt) {
    std::vector<GpuSample> out;
    auto cur = scan(procRoot);

    for (const auto& [pdev, tot] : cur) {
        if (tot.driver == "nvidia") continue;  // NVML owns NVIDIA

        GpuSample g;
        g.vendor = vendorFromDriver(tot.driver);
        g.source = "fdinfo";
        g.id = "drm:" + pdev;
        std::string vendorName =
            g.vendor == GpuVendor::Amd ? "AMD" :
            g.vendor == GpuVendor::Intel ? "Intel" :
            g.vendor == GpuVendor::Nvidia ? "NVIDIA" : "GPU";
        g.name = vendorName + " GPU (" + (tot.driver.empty() ? "drm" : tot.driver) + ")";

        if (hasPrev_ && dt > 0.0) {
            auto pit = prev_.find(pdev);
            double maxUtil = 0.0;
            bool any = false;
            if (pit != prev_.end()) {
                for (const auto& [eng, curNs] : tot.engines) {
                    auto eit = pit->second.engines.find(eng);
                    uint64_t prevNs = (eit != pit->second.engines.end()) ? eit->second : curNs;
                    maxUtil = std::max(maxUtil, engineUtil(prevNs, curNs, dt));
                    any = true;
                }
            }
            g.util_pct = any ? maxUtil : -1.0;
        } else {
            g.util_pct = -1.0;  // first sample: no delta yet
        }

        double t = readDeviceTempC(sysRoot, pdev);
        if (t >= 0.0) g.temp_c = t;

        out.push_back(std::move(g));
    }

    prev_ = std::move(cur);
    hasPrev_ = true;
    return out;
}

} // namespace sysperf
