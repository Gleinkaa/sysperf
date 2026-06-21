// worker.h — the 1 Hz collector that lives on a dedicated thread. The GUI thread
// only paints; it can never freeze, even if a collector (e.g. the fdinfo walk)
// stalls. See docs/ARCHITECTURE.md for the load-bearing threading invariants.
#pragma once

#include <QObject>

#include "config.h"
#include "collector/cpu.h"
#include "collector/diskstats.h"
#include "collector/gpu_fdinfo.h"
#include "collector/gpu_nvml.h"
#include "collector/meminfo.h"
#include "collector/netdev.h"
#include "collector/self_metrics.h"
#include "model/snapshot_qt.h"

class QTimer;

namespace sysperf {

class CollectorWorker : public QObject {
    Q_OBJECT
public:
    explicit CollectorWorker(AppConfig cfg, QObject* parent = nullptr);

public slots:
    // Connected to QThread::started — so the QTimer is created and owned INSIDE the
    // worker thread, not on the GUI thread where this object was constructed.
    void start();
    void stop();

signals:
    void snapshotReady(sysperf::Snapshot snap);  // emitted BY VALUE (queued connection)

private slots:
    void tick();

private:
    static double monotonicSeconds();

    AppConfig cfg_;
    QTimer* timer_ = nullptr;
    double lastMono_ = 0.0;
    bool first_ = true;
    long clkTck_ = 100;
    int ticksSinceWarn_ = 0;

    CpuCollector cpu_;
    MemCollector mem_;
    DiskCollector disk_;
    NetCollector net_;
    NvmlGpu nvml_;
    FdinfoGpu fdinfo_;
    SelfCollector self_;
};

} // namespace sysperf
