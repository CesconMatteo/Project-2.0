#include "charttab.h"


ChartTab::ChartTab(QWidget* parent) : QWidget(parent) {
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(57,57,57));
    setPalette(pal);
}

QFrame* ChartTab::createSeparator () {
    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    return separator;
}

void ChartTab::zeroDataTab (const QString& string) {
        zeroDataLabel = new QLabel(string);
        zeroDataLabel->setAlignment(Qt::AlignCenter);
        zeroDataLabel->setStyleSheet("font: 20pt;");
}

QPushButton* ChartTab::getNewChartDataBtn() const {
    return newChartDataBtn;
}

QPushButton* ChartTab::getDelChartDataBtn() const {
    return delChartDataBtn;
}

QString ChartTab::delChartDataDialog() {

    if (voidChart)
        return "";

    bool ok = false;
    QStringList items;
    for (auto i: chartDataNames)
        items << i->text();
    QString x = QInputDialog::getItem(this, "Eliminazione", "Seleziona quale eliminare: ", items, 0, false, &ok, Qt::Dialog);
    if (ok)
        return x;
    return "";
}

void ChartTab::exportPDF (const QString& folder) {

    QString filename = QFileDialog::getSaveFileName(this, "Salva in", folder, "PDF files(*.pdf)");
    if (filename.isEmpty())
        return;
    if (!filename.endsWith(".pdf"))
        filename = filename.append(".pdf");
    QPdfWriter writer(filename);
    writer.setPageSize(QPagedPaintDevice::A4);
    QTextDocument doc;
    QPainter painter(&writer);
    chartView->render(&painter);
    doc.drawContents(&painter);

}

const QList<QPushButton*>& ChartTab::getChartDataOptionButtons() const {
    return chartDataOptionButtons;
}

const QList<QList<QPushButton*>>& ChartTab::getSubOptionButtons() const {
    return subOptionButtons;
}

void ChartTab::execMenu() {
    menu->exec();
}

QPair<QString,QString> ChartTab::showChartDataOptions (QPushButton* sender) {
    QMenu menu("Opzioni", this);
    QAction* modifyAction = new QAction("Modifica", this);
    QAction* deleteAction = new QAction("Elimina", this);
    menu.addAction(modifyAction);
    menu.addAction(deleteAction);
    QPoint a = sender->mapToGlobal(QPoint(sender->width()/2, sender->height()/2));
    menu.move(a.x(), a.y());
    QAction* x = menu.exec();
    if (x) {
        for (int i=0; i < chartDataOptionButtons.size(); i++)
            if (chartDataOptionButtons.at(i) == sender)
                return QPair<QString,QString>(x->text(),chartDataNames.at(i)->text());
    }
    return QPair<QString,QString>();
}

QPair<QAction*, QAction*> ChartTab::showSubOptions (QPushButton*) {
    return QPair<QAction*,QAction*>(nullptr,nullptr);
}

QList<QVariant> ChartTab::modSubChartData() {
    return QList<QVariant>();
}
