#ifndef PIECHART_H
#define PIECHART_H

#include "Model/chart.h"

#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class PieChart : public Chart {
public:
    PieChart (const QString& ="");
    ~PieChart();

    bool push_back (ChartData*) final;
    bool save() const final;

    bool load (const QString&) final;
    bool operator== (PieChart) const;
    bool operator!= (PieChart) const;
};

#endif
