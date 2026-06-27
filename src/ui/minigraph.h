// minigraph.h — compact sparkline embedded in the left-side rail items.
// Like GraphWidget but stripped to essentials: no grid, no labels, just the
// colored polyline + subtle fill gradient. Shares the same ring-buffer design.
#pragma once

#include <QColor>
#include <QWidget>
#include <deque>

namespace sysperf {

class MiniGraphWidget : public QWidget {
    Q_OBJECT
public:
    explicit MiniGraphWidget(QWidget* parent = nullptr);

    void setCapacity(int samples);
    void setColor(const QColor& line, const QColor& fill);
    void addSample(double value);

    QSize sizeHint() const override { return QSize(170, 28); }

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void pushTrim(double v);

    std::deque<float> samples_;
    int capacity_ = 60;
    QColor lineColor_{0x2e, 0x9e, 0xf0};
    QColor fillColor_{0x2e, 0x9e, 0xf0, 60};
};

} // namespace sysperf
