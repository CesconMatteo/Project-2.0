#ifndef BARCHARTTAB_H
#define BARCHARTTAB_H

#include "View/charttab.h"
#include "Model/chart.h"

#include <QLabel>
#include <sstream>
#include <QScrollArea>
#include <QtCharts/QtCharts>

class BarChartTab: public ChartTab {
public:
    BarChartTab(Chart*, QWidget* =nullptr);
    void addChartData (const QStringList&) final;
    QPair<QStringList,bool> addChartDataDialog() final;
    bool delChartData (const QString&) final;
    QPair<QString,QString> modChartData (const QString&) final;
    QPair<QAction*,QAction*> showSubOptions (QPushButton*) final;
    QList<QVariant> modSubChartData() final;

    QString addCategory();
    QString delCategory();

    QPushButton* getNewCatBtn() const;
    QPushButton* getDelCatBtn() const;

private:
    QStringList categories;
    QPushButton* newCatBtn;
    QPushButton* delCatBtn;

    QHBoxLayout* header();
    void setupScroll (Chart*);
    void dxLayout (Chart*);
    void resizeAxis();
};

#endif
