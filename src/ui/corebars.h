// corebars.h — a compact grid of per-logical-CPU utilisation cells, echoing Task
// Manager's "logical processors" view. Custom paintEvent; no per-core graphs (that
// would be N widgets) — just N small filled cells, repainted on each 1 Hz sample.
#pragma once

#include <QColor>
#include <QVector>
#include <QWidget>

namespace sysperf {

class CoreBars : public QWidget {
    Q_OBJECT
public:
    explicit CoreBars(QWidget* parent = nullptr);
    void setValues(const QVector<double>& perCorePct);  // each 0..100
    void setColor(const QColor& c) { color_ = c; }
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent*) override;

private:
    QVector<double> values_;
    QColor color_{0x2e, 0x9e, 0xf0};
};

} // namespace sysperf
