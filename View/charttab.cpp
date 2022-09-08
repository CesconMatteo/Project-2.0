#include "charttab.h"


ChartTab::ChartTab(QWidget* parent) : QWidget(parent) {}

QFrame* ChartTab::createSeparator () {
    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    return separator;
}

QHBoxLayout* ChartTab::zeroDataTab (const QString& string) {
        QHBoxLayout* layout = new QHBoxLayout();
        QLabel* label = new QLabel(string);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("font: 20pt;");
        layout->addWidget(label);
        return layout;
}

void ChartTab::resizeEvent(QResizeEvent*) {
    if (!voidChart)
        scroll->setFixedWidth(subOptionButtons.at(0).at(0)->geometry().topRight().x() + (scroll->verticalScrollBar()->width() * 2));
}

QPushButton* ChartTab::getNewChartDataBtn() const {
    return newChartDataBtn;
}

QPushButton* ChartTab::getDelChartDataBtn() const {
    return delChartDataBtn;
}

QString ChartTab::delChartDataDialog() {
    bool ok = false;
    QStringList items;
    for (auto i: chartDataNames)
        items << i->text();
    QString x = QInputDialog::getItem(this, "Eliminazione", "Seleziona quale eliminare: ", items, 0, false, &ok, Qt::Dialog);
    if (ok)
        return x;
    return "";
}
