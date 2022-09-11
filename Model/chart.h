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
    /* OPERAZIONI DEL QLIST */
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

    /* METODI SPECIFICI */
    virtual bool save() const =0;
    virtual bool load (const QString&) =0;

    /* GETTERS-SETTERS */
    const QString& getPath() const;
    void setPath (const QString&);

    ChartData*& operator[](const int&);

/*
    virtual void eraseSingleElement (const int&, const int& =0) =0;
    virtual bool modify (ChartData*, const int&, const int& =0) =0;
    virtual void addSet (QString) =0;
*/
protected:
    QList<ChartData*> data;
    QString path;
};

#endif
