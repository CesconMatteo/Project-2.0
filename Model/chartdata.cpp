#include "chartdata.h"

ChartData::ChartData(const QString& _name) : name(_name) {}

ChartData::~ChartData() {}

const QString& ChartData::getName() const {
    return name;
}

void ChartData::setName (const QString& _name) {
    name = _name;
}

