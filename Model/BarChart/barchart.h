#ifndef BARCHART_H
#define BARCHART_H

#include "Model/chart.h"

#include <QStringList>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


class BarChart : public Chart {
private:
    QStringList _categories;
public:
    BarChart(const QString& ="");
    ~BarChart();
    bool push_back (ChartData*) final;
    bool save() const final;
    bool load (const QString&) final;
    bool operator== (BarChart) const;
    bool operator!= (BarChart) const;
    double maxValue() const;
    double minValue() const;
    const QStringList& categories() const;
    void addCategory (QString);
    void removeCategory (const int&);
};

#endif
