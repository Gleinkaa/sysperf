#include "ui/panel.h"

#include <QFont>
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "ui/corebars.h"
#include "ui/graphwidget.h"

namespace sysperf {

Panel::Panel(const QString& title, QWidget* parent) : QWidget(parent) {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(16, 14, 16, 14);
    root->setSpacing(10);

    titleLbl_ = new QLabel(title, this);
    {
        QFont f = titleLbl_->font();
        f.setPointSizeF(f.pointSizeF() * 1.5);
        f.setBold(true);
        titleLbl_->setFont(f);
    }
    root->addWidget(titleLbl_);

    headlineLbl_ = new QLabel(QString(), this);
    {
        QFont f = headlineLbl_->font();
        f.setPointSizeF(f.pointSizeF() * 1.25);
        headlineLbl_->setFont(f);
    }
    headlineLbl_->setVisible(false);
    root->addWidget(headlineLbl_);

    subtitleLbl_ = new QLabel(QString(), this);
    subtitleLbl_->setEnabled(false);  // grey it out
    subtitleLbl_->setVisible(false);
    root->addWidget(subtitleLbl_);

    graph_ = new GraphWidget(this);
    root->addWidget(graph_, /*stretch*/ 1);

    statsGrid_ = new QGridLayout();
    statsGrid_->setHorizontalSpacing(24);
    statsGrid_->setVerticalSpacing(6);
    root->addLayout(statsGrid_);
}

CoreBars* Panel::coreBars() {
    if (!coreBars_) {
        coreBars_ = new CoreBars(this);
        // Insert right after the graph (index varies; just add before the stats grid).
        auto* root = qobject_cast<QVBoxLayout*>(layout());
        if (root) {
            int idx = root->indexOf(graph_);
            root->insertWidget(idx + 1, coreBars_);
        }
    }
    return coreBars_;
}

void Panel::setHeadline(const QString& text) {
    headlineLbl_->setText(text);
    headlineLbl_->setVisible(!text.isEmpty());
}

void Panel::setSubtitle(const QString& text) {
    subtitleLbl_->setText(text);
    subtitleLbl_->setVisible(!text.isEmpty());
}

void Panel::addStat(const QString& key, const QString& label) {
    // Two label/value pairs per row → 4 grid columns.
    const int pairsPerRow = 2;
    const int row = statCount_ / pairsPerRow;
    const int col = (statCount_ % pairsPerRow) * 2;

    auto* nameLbl = new QLabel(label + ":", this);
    nameLbl->setEnabled(false);
    auto* valLbl = new QLabel(QStringLiteral("—"), this);
    QFont f = valLbl->font();
    f.setBold(true);
    valLbl->setFont(f);

    statsGrid_->addWidget(nameLbl, row, col);
    statsGrid_->addWidget(valLbl, row, col + 1);
    statValues_.insert(key, valLbl);
    ++statCount_;
}

void Panel::setStat(const QString& key, const QString& value) {
    auto it = statValues_.find(key);
    if (it != statValues_.end()) it.value()->setText(value);
}

} // namespace sysperf
