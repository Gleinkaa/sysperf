#include "collector/worker.h"

#include <QTimer>

#include <ctime>
#include <unistd.h>

namespace sysperf {

CollectorWorker::CollectorWorker(AppConfig cfg, QObject* parent)
    : QObject(parent), cfg_(std::move(cfg)) {
    long t = ::sysconf(_SC_CLK_TCK);
    clkTck_ = (t > 0) ? t : 100;
    // NOTE: the QTimer is intentionally NOT created here. The constructor runs on
    // the GUI thread (this object is created before moveToThread). Creating the
    // timer here would tie it to the GUI thread's event loop. It is created in
    // start(), which runs after the object has been moved to the worker thread.
}

double CollectorWorker::monotonicSeconds() {
    struct timespec ts {};
    if (::clock_gettime(CLOCK_MONOTONIC, &ts) != 0) return 0.0;
    return static_cast<double>(ts.tv_sec) + static_cast<double>(ts.tv_nsec) / 1.0e9;
}

void CollectorWorker::start() {
    if (timer_) return;
    timer_ = new QTimer(this);  // parented to this → lives in the worker thread
    timer_->setTimerType(Qt::CoarseTimer);
    connect(timer_, &QTimer::timeout, this, &CollectorWorker::tick);
    tick();  // prime the counters immediately (first sample has dt = 0)
    timer_->start(cfg_.interval_ms);
}

void CollectorWorker::stop() {
    if (timer_) {
        timer_->stop();
        timer_->deleteLater();
        timer_ = nullptr;
    }
}

void CollectorWorker::tick() {
    const double now = monotonicSeconds();
    const double dt = first_ ? 0.0 : (now - lastMono_);

    Snapshot s;
    s.mono_time = now;
    s.dt = dt;
    s.first = first_;

    s.cpu = cpu_.update(cfg_.proc_root, cfg_.sys_root);
    s.mem = mem_.update(cfg_.proc_root);
    s.disks = disk_.update(cfg_.proc_root, cfg_.sys_root, dt);
    s.nets = net_.update(cfg_.proc_root, cfg_.sys_root, dt);

    // GPUs: NVML for NVIDIA, DRM fdinfo for everything else. Either may be empty;
    // both degrade to nothing on machines without that hardware/library.
    s.gpus = nvml_.sample();
    {
        auto drm = fdinfo_.update(cfg_.proc_root, cfg_.sys_root, dt);
        s.gpus.insert(s.gpus.end(), drm.begin(), drm.end());
    }

    BudgetLimits limits{cfg_.pss_mb_warn, cfg_.cpu_pct_warn};
    s.self = self_.update(cfg_.proc_root, dt, limits, clkTck_);

    // Soft alarm, rate-limited to ~once per 5 s so logs don't flood.
    if (s.self.over_budget) {
        if (ticksSinceWarn_ <= 0) {
            qWarning("sysperf: over soft budget — PSS=%.1f MB, CPU=%.2f%% of one core "
                     "(limits: %.0f MB / %.2f%%)",
                     static_cast<double>(s.self.pss_bytes) / (1024.0 * 1024.0), s.self.cpu_pct,
                     cfg_.pss_mb_warn, cfg_.cpu_pct_warn);
            ticksSinceWarn_ = (cfg_.interval_ms > 0) ? (5000 / cfg_.interval_ms) : 5;
        } else {
            --ticksSinceWarn_;
        }
    } else {
        ticksSinceWarn_ = 0;
    }

    lastMono_ = now;
    first_ = false;
    emit snapshotReady(s);
}

} // namespace sysperf
