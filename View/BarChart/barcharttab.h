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
    bool delChartData (const QString&) final;
private:
    QHBoxLayout* header();
    void setupScroll (Chart*);
    QChartView* dxLayout (Chart*);
};

#endif
