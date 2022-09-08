#include "line.h"

Line::Line(const QString& _name) : ChartData(_name) {}

const QPair<double,double> Line::at (const int& i) const {
    return points.at(i);
}

QList<QPair<double,double>>::Iterator Line::begin() {
    return points.begin();
}

QList<QPair<double,double>>::Iterator Line::end() {
    return points.end();
}

QList<QPair<double,double>>::ConstIterator Line::cbegin() const {
    return points.cbegin();
}

QList<QPair<double,double>>::ConstIterator Line::cend() const {
    return points.cend();
}

int Line::size() const {
    return points.size();
}

void Line::clear() {
    points.clear();
}

bool Line::empty() const {
    return points.empty();
}

void Line::push_back(const QPair<double,double>& x) {
    points.push_back(x);
}

void Line::erase(QList<QPair<double,double>>::ConstIterator it) {
    points.erase(it);
}

void Line::replace (const int& pos, const QPair<double,double>& x) {
    points.replace(pos, x);
}

bool Line::operator== (Line right) const {
    return points == right.points;
}

bool Line::operator!= (Line right) const {
    return points != right.points;
}

/* COORDINATA X PIU' GRANDE */
double Line::maxX() const {
    double max = std::numeric_limits<double>::min();
    for (auto i: points)
        if (i.first > max)
            max = i.first;
    return max;
}

/* COORDINATA X PIU' PICCOLA */
double Line::minX() const {
    double min = std::numeric_limits<double>::max();
    for (auto i: points)
        if (i.first < min)
            min = i.first;
    return min;
}

/* COORDINATA Y PIU' GRANDE */
double Line::maxY() const {
    double max = std::numeric_limits<double>::min();
    for (auto i: points)
        if (i.second > max)
            max = i.second;
    return max;
}

/* COORDINATA Y PIU' PICCOLA */
double Line::minY() const {
    double min = std::numeric_limits<double>::max();
    for (auto i: points)
        if (i.second < min)
            min = i.second;
    return min;
}
