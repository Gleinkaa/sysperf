// main.cpp — process entry. Builds the worker thread and either the GUI or a
// CLI reporter, wires them with a queued, by-value Snapshot signal, and shuts
// the thread down cleanly on exit.
//
// Threading contract (see docs/ARCHITECTURE.md):
//   • qRegisterMetaType<Snapshot>() BEFORE the first queued emit, or it silently drops.
//   • worker is moveToThread'd; its QTimer is created in start() (slot on the worker
//     thread), never in the constructor (which runs here, on the GUI thread).
//   • Snapshot crosses the thread boundary BY VALUE (immutable copy).
#include <QApplication>
#include <QCoreApplication>
#include <QIcon>
#include <QMetaObject>
#include <QThread>

#include <cstdio>
#include <cstdlib>

#include "cli_output.h"
#include "collector/worker.h"
#include "config.h"
#include "model/snapshot_qt.h"
#include "ui/mainwindow.h"
#include "version.h"

using namespace sysperf;

int main(int argc, char** argv) {
    // Lightweight pre-GUI flag handling (so --help/--version don't open a window).
    bool noGui = false;
    for (int i = 1; i < argc; ++i) {
        const QString a = QString::fromLocal8Bit(argv[i]);
        if (a == "--help" || a == "-h") {
            std::printf(
                "sysperf %s — lightweight Linux system-performance monitor (Performance tab)\n\n"
                "Usage: sysperf [options]\n"
                "  --proc-root DIR   procfs root to read (default /proc; point at a fixture tree)\n"
                "  --sys-root DIR    sysfs root to read (default /sys)\n"
                "  --interval-ms N   sample period in ms (default 1000)\n"
                "  --config FILE     JSON config file (default ./config/default.json)\n"
                "  --no-gui          run in terminal-only mode (no Qt window)\n"
                "  --version         print version and exit\n"
                "  -h, --help        print this help and exit\n",
                SYSPERF_VERSION);
            return 0;
        }
        if (a == "--version") {
            std::printf("sysperf %s (%s, built %s)\n", SYSPERF_VERSION, SYSPERF_GIT_SHA,
                        SYSPERF_BUILT_AT);
            return 0;
        }
        if (a == "--no-gui") noGui = true;
    }

    // ── GUI or headless? ─────────────────────────────────────────────────────
    QCoreApplication* appPtr = nullptr;
    if (noGui) {
        appPtr = new QCoreApplication(argc, argv);
    } else {
        auto* qapp = new QApplication(argc, argv);
        qapp->setWindowIcon(QIcon(QStringLiteral(":/app-icon")));
        appPtr = qapp;
    }
    QCoreApplication::setApplicationName(QStringLiteral("sysperf"));
    QCoreApplication::setApplicationVersion(QString::fromLatin1(SYSPERF_VERSION));
    QCoreApplication::setOrganizationName(QStringLiteral("sysperf"));

    qRegisterMetaType<sysperf::Snapshot>("sysperf::Snapshot");

    const AppConfig cfg = AppConfig::load(appPtr->arguments());

    auto* thread = new QThread;
    auto* worker = new CollectorWorker(cfg);  // no parent → eligible for moveToThread
    worker->moveToThread(thread);
    QObject::connect(thread, &QThread::started, worker, &CollectorWorker::start);
    QObject::connect(thread, &QThread::finished, worker, &QObject::deleteLater);

    MainWindow* win = nullptr;
    if (noGui) {
        // CLI mode: print header, then wire snapshotReady to stdout.
        std::printf("══ sysperf %s — CLI mode (interval %d ms) ══\n"
                    "  Press Ctrl+C to exit.\n\n",
                    SYSPERF_VERSION, cfg.interval_ms);
        QObject::connect(worker, &CollectorWorker::snapshotReady, appPtr,
                         [](const Snapshot& s) {
                             std::string out = cli::render(s);
                             if (!out.empty()) {
                                 std::printf("%s\n", out.c_str());
                                 std::fflush(stdout);
                             }
                         },
                         Qt::QueuedConnection);
    } else {
        win = new MainWindow(cfg);
        QObject::connect(worker, &CollectorWorker::snapshotReady, win, &MainWindow::onSnapshot,
                         Qt::QueuedConnection);
    }

    thread->start();
    if (win) win->show();

    const int rc = appPtr->exec();

    // Graceful shutdown: stop the timer on the worker thread, then unwind it.
    QMetaObject::invokeMethod(worker, "stop", Qt::BlockingQueuedConnection);
    thread->quit();
    thread->wait();
    delete thread;  // worker was deleteLater'd via finished()
    delete win;
    delete appPtr;
    return rc;
}
