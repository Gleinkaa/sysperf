// panel.h — one Performance-tab page: a title, a big sparkline (GraphWidget), an
// optional per-core grid (CPU only), and a grid of "label: value" stat readouts the
// MainWindow updates each tick. Pure layout glue; owns no collector state.
#pragma once

#include <QHash>
#include <QString>
#include <QWidget>

namespace sysperf {

class GraphWidget;
class CoreBars;
class QGridLayout;
class QLabel;

class Panel : public QWidget {
    Q_OBJECT
public:
    explicit Panel(const QString& title, QWidget* parent = nullptr);

    GraphWidget* graph() const { return graph_; }
    CoreBars* coreBars();  // lazily creates + inserts the per-core grid

    void setHeadline(const QString& text);  // big number under the title (e.g. "23 %")
    void setSubtitle(const QString& text);   // small grey text (e.g. CPU model)

    void addStat(const QString& key, const QString& label);
    void setStat(const QString& key, const QString& value);

private:
    QLabel* titleLbl_ = nullptr;
    QLabel* headlineLbl_ = nullptr;
    QLabel* subtitleLbl_ = nullptr;
    GraphWidget* graph_ = nullptr;
    CoreBars* coreBars_ = nullptr;
    QGridLayout* statsGrid_ = nullptr;
    QHash<QString, QLabel*> statValues_;
    int statCount_ = 0;
};

} // namespace sysperf
