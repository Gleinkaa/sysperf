#include "ui/graphwidget.h"

#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QPen>

#include <algorithm>
#include <cmath>

#include "ui/format.h"

namespace sysperf {

GraphWidget::GraphWidget(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(120);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GraphWidget::setCapacity(int samples) {
    capacity_ = std::max(2, samples);
}
void GraphWidget::setFixedMax(double maxValue) {
    fixedMax_ = maxValue;
    autoScale_ = false;
}
void GraphWidget::setAutoScale(bool on) { autoScale_ = on; }
void GraphWidget::setPrimaryColor(const QColor& line, const QColor& fill) {
    lineColor_ = line;
    fillColor_ = fill;
}
void GraphWidget::setSecondaryColor(const QColor& line) {
    line2Color_ = line;
    hasSecondary_ = true;
}
void GraphWidget::enableSecondary(bool on) { hasSecondary_ = on; }
void GraphWidget::setUnitSuffix(const QString& s) { unitSuffix_ = s; }

void GraphWidget::pushTrim(std::deque<float>& buf, double v) {
    buf.push_back(static_cast<float>(v));
    while (static_cast<int>(buf.size()) > capacity_) buf.pop_front();
}

void GraphWidget::addSample(double primary) {
    pushTrim(primary_, primary);
    if (isVisible()) update();
}

void GraphWidget::addSample(double primary, double secondary) {
    pushTrim(primary_, primary);
    pushTrim(secondary_, secondary);
    hasSecondary_ = true;
    if (isVisible()) update();
}

double GraphWidget::currentMax() const {
    if (!autoScale_) return fixedMax_ > 0.0 ? fixedMax_ : 1.0;
    double m = 0.0;
    for (float v : primary_) m = std::max(m, static_cast<double>(v));
    for (float v : secondary_) m = std::max(m, static_cast<double>(v));
    if (m <= 0.0) return 1.0;
    // round up to a "nice" headroom so the line doesn't kiss the top edge
    return m * 1.15;
}

void GraphWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    const QRectF r = rect().adjusted(0.0, 0.0, -1.0, -1.0);
    const double w = r.width();
    const double h = r.height();
    if (w <= 2 || h <= 2) return;

    const QPalette& pal = palette();
    QColor gridColor = pal.color(QPalette::WindowText);
    gridColor.setAlpha(36);
    QColor bg = pal.color(QPalette::Base);

    p.fillRect(r, bg);

    // ── Grid: 1px device-pixel-crisp lines, NOT antialiased (per the plan). ──
    p.setRenderHint(QPainter::Antialiasing, false);
    QPen gridPen(gridColor);
    gridPen.setWidth(0);  // cosmetic → exactly 1 device pixel at any DPR
    p.setPen(gridPen);
    const int cols = 6, rows = 4;
    for (int c = 1; c < cols; ++c) {
        double x = r.left() + std::round(w * c / cols) + 0.5;  // +0.5 ⇒ crisp
        p.drawLine(QPointF(x, r.top()), QPointF(x, r.bottom()));
    }
    for (int rr = 1; rr < rows; ++rr) {
        double y = r.top() + std::round(h * rr / rows) + 0.5;
        p.drawLine(QPointF(r.left(), y), QPointF(r.right(), y));
    }
    p.setPen(gridPen);
    p.drawRect(r);

    const double maxV = currentMax();

    auto buildPath = [&](const std::deque<float>& buf, QPainterPath& line, QPainterPath& fill) {
        const int n = static_cast<int>(buf.size());
        if (n < 1) return false;
        // Newest sample anchored at the right edge; map [0..capacity-1] across width.
        auto xAt = [&](int i) {
            double frac = (capacity_ > 1) ? static_cast<double>(i) / (capacity_ - 1) : 1.0;
            return r.left() + frac * w;
        };
        auto yAt = [&](double v) {
            double frac = std::clamp(v / maxV, 0.0, 1.0);
            return r.bottom() - frac * h;
        };
        const int startSlot = capacity_ - n;  // right-align
        line.moveTo(xAt(startSlot), yAt(buf.front()));
        for (int i = 1; i < n; ++i) line.lineTo(xAt(startSlot + i), yAt(buf[i]));
        fill = line;
        fill.lineTo(xAt(startSlot + n - 1), r.bottom());
        fill.lineTo(xAt(startSlot), r.bottom());
        fill.closeSubpath();
        return true;
    };

    // ── Data: antialiased polyline + gradient fill. ──
    p.setRenderHint(QPainter::Antialiasing, true);

    if (hasSecondary_ && !secondary_.empty()) {
        QPainterPath l2, f2;
        if (buildPath(secondary_, l2, f2)) {
            QPen pen2(line2Color_);
            pen2.setWidthF(1.6);
            p.setPen(pen2);
            p.setBrush(Qt::NoBrush);
            p.drawPath(l2);
        }
    }

    QPainterPath line, fill;
    if (buildPath(primary_, line, fill)) {
        QLinearGradient grad(0, r.top(), 0, r.bottom());
        QColor top = fillColor_;
        top.setAlpha(110);
        QColor bot = fillColor_;
        bot.setAlpha(8);
        grad.setColorAt(0.0, top);
        grad.setColorAt(1.0, bot);
        p.setPen(Qt::NoPen);
        p.setBrush(grad);
        p.drawPath(fill);

        QPen linePen(lineColor_);
        linePen.setWidthF(1.8);
        linePen.setJoinStyle(Qt::RoundJoin);
        p.setPen(linePen);
        p.setBrush(Qt::NoBrush);
        p.drawPath(line);
    }

    // ── Max-value label (top-left), antialiased text. ──
    QColor txt = pal.color(QPalette::WindowText);
    txt.setAlpha(150);
    p.setPen(txt);
    QString label;
    if (autoScale_) {
        label = (unitSuffix_ == "B/s") ? fmt::rate(maxV) : QString::number(maxV, 'f', 0) + unitSuffix_;
    } else {
        label = QString::number(maxV, 'f', 0) + unitSuffix_;
    }
    p.drawText(r.adjusted(4, 2, -4, -2), Qt::AlignTop | Qt::AlignLeft, label);
    p.drawText(r.adjusted(4, 2, -4, -2), Qt::AlignBottom | Qt::AlignLeft, "0" + unitSuffix_);
    p.drawText(r.adjusted(4, 2, -4, -2), Qt::AlignBottom | Qt::AlignRight,
               QString("%1s").arg(capacity_));
}

} // namespace sysperf
