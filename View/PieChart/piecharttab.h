#ifndef PIECHARTTAB_H
#define PIECHARTTAB_H

#include "View/charttab.h"
#include "Model/chart.h"

#include <sstream>
#include <math.h>
#include <QLabel>
#include <QScrollArea>
#include <QtCharts/QtCharts>
#include <QString>

class PieChartTab: public ChartTab {
    Q_OBJECT
public:
    PieChartTab(Chart*, QWidget* =nullptr);
    bool delChartData (const QString&) final;
private:

    QHBoxLayout* header();
    void setupScroll (Chart*);
    QChartView* dxLayout (Chart*);
    void updatePercentage();
private slots:
    void explodeSlice (bool);
};

#endif
