// graphwidget.h — the custom sparkline. A plain QWidget whose paintEvent draws a
// QPainterPath polyline + filled gradient from a ring buffer it owns. There is NO
// QtCharts, NO QOpenGLWidget, and NO animation/FPS loop: update() is called only
// when a new ~1 Hz sample arrives, and only while the widget is visible. The ring
// buffer keeps filling even when hidden, so history is intact when the tab is shown.
#pragma once

#include <QColor>
#include <QWidget>
#include <deque>

namespace sysperf {

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    explicit GraphWidget(QWidget* parent = nullptr);

    void setCapacity(int samples);          // ring-buffer length (≈ history seconds)
    void setFixedMax(double maxValue);       // e.g. 100 for percent graphs
    void setAutoScale(bool on);              // grow the y-axis to the data (rates)
    void setPrimaryColor(const QColor& line, const QColor& fill);
    void setSecondaryColor(const QColor& line);  // enables a 2nd overlaid series
    void setUnitSuffix(const QString& s);    // drawn on the autoscale max label
    void enableSecondary(bool on);

    // Append a new sample. Always stored (history fills while hidden); repaints
    // only if visible.
    void addSample(double primary);
    void addSample(double primary, double secondary);

    QSize sizeHint() const override { return QSize(480, 180); }

protected:
    void paintEvent(QPaintEvent*) override;

private:
    double currentMax() const;
    void pushTrim(std::deque<float>& buf, double v);

    std::deque<float> primary_;
    std::deque<float> secondary_;
    bool hasSecondary_ = false;

    int capacity_ = 60;
    double fixedMax_ = 100.0;
    bool autoScale_ = false;

    QColor lineColor_{0x2e, 0x9e, 0xf0};
    QColor fillColor_{0x2e, 0x9e, 0xf0, 60};
    QColor line2Color_{0xf0, 0x7e, 0x2e};
    QString unitSuffix_;
};

} // namespace sysperf
