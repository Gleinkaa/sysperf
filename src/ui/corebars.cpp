#include "ui/corebars.h"

#include <QPainter>
#include <QPalette>

#include <algorithm>
#include <cmath>

namespace sysperf {

CoreBars::CoreBars(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(46);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

QSize CoreBars::sizeHint() const { return QSize(480, 60); }

void CoreBars::setValues(const QVector<double>& perCorePct) {
    values_ = perCorePct;
    if (isVisible()) update();
}

void CoreBars::paintEvent(QPaintEvent*) {
    const int n = values_.size();
    if (n <= 0) return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);
    const QPalette& pal = palette();
    QColor cellBg = pal.color(QPalette::Base);
    QColor border = pal.color(QPalette::WindowText);
    border.setAlpha(40);

    // Choose a column count that keeps cells roughly square-ish.
    const int cols = std::min(n, std::max(1, static_cast<int>(std::ceil(std::sqrt(n * 2.0)))));
    const int rows = (n + cols - 1) / cols;

    const double gap = 3.0;
    const double cw = (width() - gap * (cols + 1)) / cols;
    const double ch = (height() - gap * (rows + 1)) / rows;
    if (cw <= 1 || ch <= 1) return;

    for (int i = 0; i < n; ++i) {
        const int rIdx = i / cols;
        const int cIdx = i % cols;
        const double x = gap + cIdx * (cw + gap);
        const double y = gap + rIdx * (ch + gap);
        QRectF cell(x, y, cw, ch);

        p.fillRect(cell, cellBg);
        // fill from the bottom proportional to utilisation
        double frac = std::clamp(values_[i] / 100.0, 0.0, 1.0);
        QRectF fillRect(cell.left(), cell.bottom() - frac * ch, cell.width(), frac * ch);
        QColor c = color_;
        c.setAlpha(190);
        p.fillRect(fillRect, c);

        p.setPen(QPen(border, 0));
        p.drawRect(cell);
    }
}

} // namespace sysperf
