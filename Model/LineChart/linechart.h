#ifndef LINECHART_H
#define LINECHART_H

#include "Model/chart.h"

#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class LineChart : public Chart {

public:
    LineChart(const QString& ="");
    ~LineChart();
    bool push_back (ChartData*) final;
    bool save() const final;
    bool load (const QString&) final;
    bool operator== (LineChart) const;
    bool operator!= (LineChart) const;
};
#endif
