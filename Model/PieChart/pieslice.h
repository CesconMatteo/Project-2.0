#ifndef PIESLICE_H
#define PIESLICE_H

#include "Model/chartdata.h"

#include <QString>

class PieSlice : public ChartData {
private:
    double value;
public:
    PieSlice (const QString& _name ="", const double& _value =0);
    const double& getValue() const;
    void setValue (double newValue);
    bool operator== (PieSlice) const;
    bool operator!= (PieSlice) const;
};

#endif
