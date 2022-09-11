#ifndef LINECHARTTAB_H
#define LINECHARTTAB_H

#include "View/charttab.h"
#include "Model/chart.h"

#include <QLabel>
#include <QScrollArea>
#include <QtCharts/QtCharts>
#include <sstream>
#include <math.h>

using std::stringstream;

class LineChartTab: public ChartTab {
public:
    LineChartTab(Chart*, QWidget* =nullptr);
    void addChartData (const QStringList&) final;
    QStringList addChartDataDialog() final;
    bool delChartData (const QString&) final;
    QPair<QString,QString> showChartDataOptions (QPushButton*) final;
    QPair<QString,QString> modChartData (const QString&) final;
    QPair<QAction*,QAction*> showSubOptions (QPushButton*) final;

    QList<double> addNewPoint (const QString& chartDataName);
    QPair<QString,int> deletePoint();
    QList<QVariant> modSubChartData() final;

private:
    QHBoxLayout* header();
    void setupScroll (Chart*);
    QChartView* dxLayout (Chart*);
    void resizeAxis();
};

#endif
