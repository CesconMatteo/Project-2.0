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
    void addChartData (const QStringList&);
    QPair<QStringList,bool> addChartDataDialog() final;
    bool delChartData (const QString&) final;
    QPair<QString,QString> modChartData (const QString&) final;
private:

    QHBoxLayout* header();
    void setupScroll (Chart*);
    void setupChart (Chart*);
    void updatePercentage();
private slots:
    void explodeSlice (bool);
};

#endif
