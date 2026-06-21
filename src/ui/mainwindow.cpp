#include "ui/mainwindow.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QStackedWidget>
#include <QStatusBar>
#include <QVector>
#include <QWidget>

#include <algorithm>

#include "ui/corebars.h"
#include "ui/format.h"
#include "ui/graphwidget.h"
#include "ui/panel.h"
#include "version.h"

namespace sysperf {

namespace {
const QColor kCpuLine(0x2e, 0x9e, 0xf0), kCpuFill(0x2e, 0x9e, 0xf0);
const QColor kMemLine(0x9b, 0x59, 0xd0), kMemFill(0x9b, 0x59, 0xd0);
const QColor kDiskLine(0x27, 0xae, 0x60), kDiskFill(0x27, 0xae, 0x60);
const QColor kNetRx(0x16, 0xa0, 0x85), kNetTx(0xe0, 0x7b, 0x39);
const QColor kGpuLine(0xe0, 0x4f, 0x5f), kGpuFill(0xe0, 0x4f, 0x5f);
} // namespace

MainWindow::MainWindow(AppConfig cfg, QWidget* parent) : QMainWindow(parent), cfg_(std::move(cfg)) {
    setWindowTitle(QStringLiteral("sysperf %1").arg(QString::fromLatin1(SYSPERF_VERSION)));

    auto* central = new QWidget(this);
    auto* h = new QHBoxLayout(central);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);

    rail_ = new QListWidget(central);
    rail_->setMaximumWidth(190);
    rail_->setMinimumWidth(150);
    rail_->setSpacing(2);
    rail_->setFrameShape(QFrame::NoFrame);

    stack_ = new QStackedWidget(central);

    h->addWidget(rail_);
    h->addWidget(stack_, 1);
    setCentralWidget(central);

    connect(rail_, &QListWidget::currentRowChanged, stack_, &QStackedWidget::setCurrentIndex);

    // Status bar: self-instrumentation readout (the budget guardrail, always visible).
    selfLabel_ = new QLabel(QStringLiteral("self: —"), this);
    statusBar()->addPermanentWidget(selfLabel_);

    auto* help = menuBar()->addMenu(tr("&Help"));
    help->addAction(tr("&About sysperf"), this, &MainWindow::showAbout);

    buildStaticPanels();

    // Honour ui.start_panel (cpu | memory).
    int startRow = 0;
    if (cfg_.start_panel == "memory") startRow = 1;
    rail_->setCurrentRow(startRow);

    resize(940, 580);
}

Panel* MainWindow::ensurePanel(const QString& id, const QString& title, const QString& railName) {
    auto it = panels_.find(id);
    if (it != panels_.end()) return it.value();
    auto* p = new Panel(title);
    rail_->addItem(railName);
    stack_->addWidget(p);
    panels_.insert(id, p);
    return p;
}

void MainWindow::buildStaticPanels() {
    // CPU
    Panel* cpu = ensurePanel("cpu", tr("CPU"), tr("CPU"));
    cpu->graph()->setFixedMax(100.0);
    cpu->graph()->setCapacity(cfg_.history_seconds);
    cpu->graph()->setUnitSuffix(QStringLiteral(" %"));
    cpu->graph()->setPrimaryColor(kCpuLine, kCpuFill);
    cpu->coreBars()->setColor(kCpuLine);
    cpu->addStat("util", tr("Utilization"));
    cpu->addStat("speed", tr("Speed"));
    cpu->addStat("cores", tr("Logical cores"));
    cpu->addStat("max_core", tr("Busiest core"));

    // Memory
    Panel* mem = ensurePanel("mem", tr("Memory"), tr("Memory"));
    mem->graph()->setFixedMax(100.0);
    mem->graph()->setCapacity(cfg_.history_seconds);
    mem->graph()->setUnitSuffix(QStringLiteral(" %"));
    mem->graph()->setPrimaryColor(kMemLine, kMemFill);
    mem->addStat("inuse", tr("In use"));
    mem->addStat("avail", tr("Available"));
    mem->addStat("cached", tr("Cached"));
    mem->addStat("committed", tr("Committed"));
    mem->addStat("swap", tr("Swap"));
    mem->addStat("slab", tr("Slab (recl/unrecl)"));
}

void MainWindow::onSnapshot(Snapshot s) {
    updateCpu(s);
    updateMem(s);
    updateDisks(s);
    updateNets(s);
    updateGpus(s);
    updateSelf(s);
}

void MainWindow::updateCpu(const Snapshot& s) {
    Panel* p = panels_.value("cpu");
    if (!p) return;
    const CpuInfo& c = s.cpu;
    p->graph()->addSample(c.total_pct);

    QVector<double> cores;
    cores.reserve(static_cast<int>(c.per_core_pct.size()));
    double maxCore = 0.0;
    for (double v : c.per_core_pct) {
        cores.push_back(v);
        maxCore = std::max(maxCore, v);
    }
    p->coreBars()->setValues(cores);

    p->setHeadline(fmt::percent(c.total_pct));
    if (!c.model.empty())
        p->setSubtitle(QString::fromStdString(c.model));
    p->setStat("util", fmt::percent(c.total_pct, 1));
    p->setStat("speed", fmt::ghz(c.freq_ghz));
    p->setStat("cores", QString::number(c.logical_cores));
    p->setStat("max_core", fmt::percent(maxCore, 0));
}

void MainWindow::updateMem(const Snapshot& s) {
    Panel* p = panels_.value("mem");
    if (!p) return;
    const MemInfo& m = s.mem;
    double usedPct = (m.total > 0) ? 100.0 * static_cast<double>(m.used) / static_cast<double>(m.total) : 0.0;
    p->graph()->addSample(usedPct);

    p->setHeadline(QStringLiteral("%1 / %2").arg(fmt::bytes(m.used), fmt::bytes(m.total)));
    p->setStat("inuse", fmt::bytes(m.used));
    p->setStat("avail", fmt::bytes(m.available));
    p->setStat("cached", fmt::bytes(m.cached));
    p->setStat("committed",
               QStringLiteral("%1 / %2").arg(fmt::bytes(m.committed), fmt::bytes(m.commit_limit)));
    uint64_t swapUsed = (m.swap_total > m.swap_free) ? (m.swap_total - m.swap_free) : 0ULL;
    p->setStat("swap", QStringLiteral("%1 / %2").arg(fmt::bytes(swapUsed), fmt::bytes(m.swap_total)));
    p->setStat("slab", QStringLiteral("%1 / %2").arg(fmt::bytes(m.s_reclaimable),
                                                     fmt::bytes(m.s_unreclaim)));
}

void MainWindow::updateDisks(const Snapshot& s) {
    for (const DiskSample& d : s.disks) {
        const QString name = QString::fromStdString(d.name);
        const QString id = "disk:" + name;
        bool created = !panels_.contains(id);
        Panel* p = ensurePanel(id, tr("Disk — %1").arg(name), tr("Disk (%1)").arg(name));
        if (created) {
            p->graph()->setFixedMax(100.0);
            p->graph()->setCapacity(cfg_.history_seconds);
            p->graph()->setUnitSuffix(QStringLiteral(" %"));
            p->graph()->setPrimaryColor(kDiskLine, kDiskFill);
            p->addStat("active", tr("Active time"));
            p->addStat("read", tr("Read"));
            p->addStat("write", tr("Write"));
            p->addStat("type", tr("Type"));
        }
        p->graph()->addSample(d.util_pct);
        p->setHeadline(fmt::percent(d.util_pct, 1));
        p->setStat("active", fmt::percent(d.util_pct, 1));
        p->setStat("read", fmt::rate(d.read_bps));
        p->setStat("write", fmt::rate(d.write_bps));
        p->setStat("type", d.has_rotational ? (d.rotational ? tr("HDD") : tr("SSD / NVMe"))
                                            : tr("—"));
    }
}

void MainWindow::updateNets(const Snapshot& s) {
    for (const NetSample& n : s.nets) {
        const QString name = QString::fromStdString(n.name);
        const QString id = "net:" + name;
        bool created = !panels_.contains(id);
        Panel* p = ensurePanel(id, tr("Network — %1").arg(name), tr("Net (%1)").arg(name));
        if (created) {
            p->graph()->setAutoScale(true);
            p->graph()->setCapacity(cfg_.history_seconds);
            p->graph()->setUnitSuffix(QStringLiteral("B/s"));
            p->graph()->setPrimaryColor(kNetRx, kNetRx);
            p->graph()->setSecondaryColor(kNetTx);
            p->addStat("recv", tr("Receive"));
            p->addStat("send", tr("Send"));
            p->addStat("link", tr("Link speed"));
        }
        p->graph()->addSample(n.rx_bps, n.tx_bps);
        p->setHeadline(tr("R %1   ·   S %2").arg(fmt::rate(n.rx_bps), fmt::rate(n.tx_bps)));
        p->setStat("recv", fmt::rate(n.rx_bps));
        p->setStat("send", fmt::rate(n.tx_bps));
        p->setStat("link", fmt::linkSpeed(n.speed_mbps));
    }
}

void MainWindow::updateGpus(const Snapshot& s) {
    for (const GpuSample& g : s.gpus) {
        const QString id = "gpu:" + QString::fromStdString(g.id.empty() ? g.name : g.id);
        const QString name = QString::fromStdString(g.name);
        bool created = !panels_.contains(id);
        Panel* p = ensurePanel(id, tr("GPU — %1").arg(name), tr("GPU"));
        if (created) {
            p->graph()->setFixedMax(100.0);
            p->graph()->setCapacity(cfg_.history_seconds);
            p->graph()->setUnitSuffix(QStringLiteral(" %"));
            p->graph()->setPrimaryColor(kGpuLine, kGpuFill);
            p->addStat("util", tr("Utilization"));
            p->addStat("mem", tr("Memory"));
            p->addStat("temp", tr("Temperature"));
            p->addStat("clock", tr("Clock"));
            p->addStat("power", tr("Power"));
            p->addStat("source", tr("Source"));
        }
        double u = (g.util_pct >= 0.0) ? g.util_pct : 0.0;
        p->graph()->addSample(u);
        p->setHeadline(g.util_pct >= 0.0 ? fmt::percent(g.util_pct, 0) : tr("—"));
        p->setStat("util", g.util_pct >= 0.0 ? fmt::percent(g.util_pct, 0) : tr("—"));
        p->setStat("mem", g.mem_total > 0
                              ? QStringLiteral("%1 / %2").arg(fmt::bytes(g.mem_used),
                                                              fmt::bytes(g.mem_total))
                              : tr("—"));
        p->setStat("temp", fmt::tempC(g.temp_c));
        p->setStat("clock", g.clock_mhz >= 0.0 ? QString::asprintf("%.0f MHz", g.clock_mhz) : tr("—"));
        p->setStat("power", fmt::watts(g.power_w));
        p->setStat("source", QString::fromStdString(g.source));
    }
}

void MainWindow::updateSelf(const Snapshot& s) {
    const SelfMetrics& m = s.self;
    selfLabel_->setText(tr("self: PSS %1  ·  CPU %2")
                            .arg(fmt::bytes(m.pss_bytes), fmt::percent(m.cpu_pct, 2)));
    selfLabel_->setStyleSheet(m.over_budget ? QStringLiteral("color:#d33;font-weight:bold;")
                                            : QString());
}

void MainWindow::showAbout() {
    QMessageBox::about(
        this, tr("About sysperf"),
        tr("<b>sysperf %1</b><br>"
           "A lightweight Linux system-performance monitor (Performance tab only).<br><br>"
           "Build: %2 · %3<br>"
           "Single unprivileged process. Honest Linux semantics.<br>"
           "Qt 6 is used under LGPLv3 (dynamic linking).")
            .arg(QString::fromLatin1(SYSPERF_VERSION), QString::fromLatin1(SYSPERF_GIT_SHA),
                 QString::fromLatin1(SYSPERF_BUILT_AT)));
}

} // namespace sysperf
