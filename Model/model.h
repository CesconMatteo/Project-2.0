#ifndef MODEL_H
#define MODEL_H

#include "chart.h"

#include <QString>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

enum ChartType { Bar, Line, Pie };

class Model {
private:
    QList<Chart*> charts;
    QList<int> saved;
    QString desktopPath;
public:
    Model();

    const QString& getDesktopPath() const;

    /* CHART RELATED */
    const Chart* chartAt (const int&) const;
    bool loadChart (const QString&);
    void newChart (ChartType);
    bool saveChart (const int&);
    bool saveAs (const int&, const QString&);
    bool isSaved (const int&) const;
    bool allSaved() const;
    void removeChart (const int&);
    Chart* last();
    bool chartFileExists (const int&) const;

    /* CHARTDATA RELATED */
    void addChartData (const int&, const QStringList&);
    bool removeChartData (const int&, const QString&);
    void modifyChartData (const int&, const int&, const QStringList&);

    /* ELEMENTS OF CHARTDATA */
    /* AGGIUNTA DI UN PUNTO/ SINGOLO DOUBLE */
};

#endif
