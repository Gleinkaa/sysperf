// mainwindow.h — the shell: a left rail of subsystems and a stacked set of Panels.
// Owns the ring buffers (inside each Panel's GraphWidget) and only paints; all
// collection happens on the worker thread and arrives via onSnapshot (queued).
#pragma once

#include <QHash>
#include <QMainWindow>
#include <QString>

#include "config.h"
#include "model/snapshot_qt.h"

class QListWidget;
class QStackedWidget;
class QLabel;

namespace sysperf {

class Panel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(AppConfig cfg, QWidget* parent = nullptr);

public slots:
    void onSnapshot(sysperf::Snapshot snap);

private:
    Panel* ensurePanel(const QString& id, const QString& title, const QString& railName);
    void buildStaticPanels();
    void updateCpu(const Snapshot& s);
    void updateMem(const Snapshot& s);
    void updateDisks(const Snapshot& s);
    void updateNets(const Snapshot& s);
    void updateGpus(const Snapshot& s);
    void updateSelf(const Snapshot& s);
    void showAbout();

    AppConfig cfg_;
    QListWidget* rail_ = nullptr;
    QStackedWidget* stack_ = nullptr;
    QLabel* selfLabel_ = nullptr;
    QHash<QString, Panel*> panels_;
};

} // namespace sysperf
