#ifndef BARSET_H
#define BARSET_H

#include "Model/chartdata.h"

#include <QList>

class BarSet : public ChartData {
private:
    QList<double> values;
public:
    BarSet(const QString&);

    const double& at (const int&) const;
    QList<double>::Iterator begin();
    QList<double>::Iterator end();
    QList<double>::ConstIterator cbegin() const;
    QList<double>::ConstIterator cend() const;
    int size() const;
    void clear();
    bool empty() const;
    void push_back (const double&);
    void erase (QList<double>::ConstIterator);
    void replace (const int&, const double&);

    double maxValue() const;
    double minValue() const;

    bool operator== (BarSet) const;
    bool operator!= (BarSet) const;
};

#endif
