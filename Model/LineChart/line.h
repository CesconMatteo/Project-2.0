#ifndef LINE_H
#define LINE_H

#include "Model/chartdata.h"

#include <QList>

class Line : public ChartData {
private:
    /* OGNI PUNTO E' RAPPRESENTANTO CON UN QPAIR, FIRST E' X E SECOND E' Y */
    QList<QPair<double,double>> points;
public:
    Line(const QString&);

    const QPair<double,double> at (const int&) const;
    QList<QPair<double,double>>::Iterator begin();
    QList<QPair<double,double>>::Iterator end();
    QList<QPair<double,double>>::ConstIterator cbegin() const;
    QList<QPair<double,double>>::ConstIterator cend() const;
    int size() const;
    void clear();
    bool empty() const;
    void push_back (const QPair<double,double>&);
    void erase (QList<QPair<double,double>>::Iterator);
    void replace (const int&, const QPair<double,double>&);
    void removeAt (const int&);

    double maxX() const;
    double minX() const;
    double maxY() const;
    double minY() const;

    bool operator== (Line) const;
    bool operator!= (Line) const;
};

#endif
