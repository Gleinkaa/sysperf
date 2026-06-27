#include "ui/minigraph.h"

#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>

#include <algorithm>

namespace sysperf {

MiniGraphWidget::MiniGraphWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(120, 24);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
}

void MiniGraphWidget::setCapacity(int samples) {
    capacity_ = std::max(2, samples);
}

void MiniGraphWidget::setColor(const QColor& line, const QColor& fill) {
    lineColor_ = line;
    fillColor_ = fill;
}

void MiniGraphWidget::pushTrim(double v) {
    samples_.push_back(static_cast<float>(v));
    while (static_cast<int>(samples_.size()) > capacity_) samples_.pop_front();
}

void MiniGraphWidget::addSample(double value) {
    pushTrim(value);
    update(); // always repaint — these are tiny, and always visible in the rail
}

void MiniGraphWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    const QRectF r = rect().adjusted(0.0, 0.0, -1.0, -1.0);
    const double w = r.width();
    const double h = r.height();
    if (w <= 2 || h <= 2) return;

    const QPalette& pal = palette();
    QColor bg = pal.color(QPalette::Base);

    p.fillRect(r, bg);

    const int n = static_cast<int>(samples_.size());
    if (n < 1) return;

    // Find max for y-scaling; start from a floor so flat lines don't vanish.
    double maxV = 0.5;
    for (float v : samples_) maxV = std::max(maxV, static_cast<double>(v));
    maxV *= 1.15; // slight headroom

    // Build polyline (right-aligned, newest at right edge).
    auto xAt = [&](int i) {
        double frac = (capacity_ > 1) ? static_cast<double>(i) / (capacity_ - 1) : 1.0;
        return r.left() + frac * w;
    };
    auto yAt = [&](double v) {
        double frac = std::clamp(v / maxV, 0.0, 1.0);
        return r.bottom() - frac * h;
    };

    const int startSlot = capacity_ - n;
    QPainterPath linePath;
    linePath.moveTo(xAt(startSlot), yAt(static_cast<double>(samples_.front())));
    for (int i = 1; i < n; ++i)
        linePath.lineTo(xAt(startSlot + i), yAt(static_cast<double>(samples_[i])));

    // Fill path: close the shape to the bottom.
    QPainterPath fillPath = linePath;
    fillPath.lineTo(xAt(startSlot + n - 1), r.bottom());
    fillPath.lineTo(xAt(startSlot), r.bottom());
    fillPath.closeSubpath();

    p.setRenderHint(QPainter::Antialiasing, true);

    // Fill gradient.
    QLinearGradient grad(0, r.top(), 0, r.bottom());
    QColor top = fillColor_;
    top.setAlpha(80);
    QColor bot = fillColor_;
    bot.setAlpha(4);
    grad.setColorAt(0.0, top);
    grad.setColorAt(1.0, bot);
    p.setPen(Qt::NoPen);
    p.setBrush(grad);
    p.drawPath(fillPath);

    // Line.
    QPen linePen(lineColor_);
    linePen.setWidthF(1.2);
    linePen.setJoinStyle(Qt::RoundJoin);
    p.setPen(linePen);
    p.setBrush(Qt::NoBrush);
    p.drawPath(linePath);
}

} // namespace sysperf
