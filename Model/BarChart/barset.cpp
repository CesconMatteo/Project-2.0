#include "barset.h"

BarSet::BarSet(const QString& _name) : ChartData(_name) {}

const double& BarSet::at (const int& i) const {
    return values.at(i);
}

QList<double>::Iterator BarSet::begin() {
    return values.begin();
}

QList<double>::Iterator BarSet::end() {
    return values.end();
}

QList<double>::ConstIterator BarSet::cbegin() const {
    return values.cbegin();
}

QList<double>::ConstIterator BarSet::cend() const {
    return values.cend();
}

int BarSet::size() const {
    return values.size();
}

void BarSet::clear() {
    values.clear();
}

bool BarSet::empty() const {
    return values.empty();
}

void BarSet::push_back(const double& x) {
    values.push_back(x);
}

void BarSet::erase(QList<double>::ConstIterator it) {
    values.erase(it);
}

void BarSet::removeAt (const int& i) {
    values.removeAt(i);
}

void BarSet::replace (const int& pos, const double& x) {
    values.replace(pos, x);
}

double BarSet::maxValue() const {
    double max = std::numeric_limits<double>::min();
    for (auto i: values)
        if (i > max)
            max = i;
    if (max != std::numeric_limits<double>::min())
        return max;
    else
        return 0;
}

double BarSet::minValue() const {
    double min = std::numeric_limits<double>::max();
    for (auto i: values)
        if (i < min)
            min = i;
    if (min != std::numeric_limits<double>::max())
        return min;
    else
        return 0;
}

bool BarSet::operator== (BarSet right) const {
    return values == right.values;
}

bool BarSet::operator!= (BarSet right) const {
    return values != right.values;
}
