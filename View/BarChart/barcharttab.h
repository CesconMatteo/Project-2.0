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
    QStringList addChartDataDialog() final;
    bool delChartData (const QString&) final;
    QPair<QString,QString> modChartData (const QString&) final;
private:
    QStringList categories;

    QHBoxLayout* header();
    void setupScroll (Chart*);
    void dxLayout (Chart*);
    void resizeAxis();
};

#endif
