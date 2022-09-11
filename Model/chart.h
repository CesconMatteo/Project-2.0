#ifndef CHART_H
#define CHART_H

#include "chartdata.h"

#include <QString>
#include <QList>
#include <QStandardPaths>

class Chart {
public:
    Chart (const QString& =0);
    virtual ~Chart();

    const ChartData* at (const int&) const;
    QList<ChartData*>::Iterator begin();
    QList<ChartData*>::Iterator end();
    QList<ChartData*>::ConstIterator cbegin() const;
    QList<ChartData*>::ConstIterator cend() const;
    int size() const;
    void clear();
    bool empty() const;
    virtual bool push_back (ChartData*) =0;
    void erase (QList<ChartData*>::Iterator);
    void replace (const int&, ChartData*);
    void removeAt (const int&);

    virtual bool save() const =0;
    virtual bool load (const QString&) =0;

    const QString& getPath() const;
    void setPath (const QString&);

protected:
    QList<ChartData*> data;
    QString path;
};

#endif
