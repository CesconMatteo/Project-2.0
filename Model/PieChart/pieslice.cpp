#include "pieslice.h"


PieSlice::PieSlice (const QString& _name, const double& _value) : ChartData(_name), value(_value) {}

const double& PieSlice::getValue() const {
    return value;
}

void PieSlice::setValue(double _value) {
    value = _value;
}

bool PieSlice::operator== (PieSlice right) const {
    return (value == right.value) && (name == right.name);
}

bool PieSlice::operator!= (PieSlice right) const {
    return (value != right.value) || (name != right.name);
}
