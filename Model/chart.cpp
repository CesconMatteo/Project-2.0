#include "chart.h"

Chart::Chart (const QString& _path) : path(_path) {
    if (path.isEmpty())
        path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/nuovo_grafico.json");
    if (path.endsWith("/"))
        path.append("nuovo_grafico.json");
    if (!path.endsWith(".json"))
        path.append(".json");
    data = QList<ChartData*>();
}

Chart::~Chart() {
    for (auto i: data)
        delete i;
}

const QString& Chart::getPath() const {
    return path;
}

void Chart::setPath (const QString& _path) {
    if (_path.isEmpty())
        return;
    path = _path;
    if (!path.endsWith(".json"))
        path = path.append(".json");
}

void Chart::clear() {
    data.clear();
}

const ChartData* Chart::at(const int& i) const {
    return data.at(i);
}

QList<ChartData*>::Iterator Chart::begin() {
    return data.begin();
}

QList<ChartData*>::Iterator Chart::end() {
    return data.end();
}

QList<ChartData*>::ConstIterator Chart::cbegin() const {
    return data.begin();
}

QList<ChartData*>::ConstIterator Chart::cend() const {
    return data.end();
}

bool Chart::empty() const {
    return data.empty();
}

void Chart::erase(QList<ChartData*>::Iterator it) {
    delete (*it);
    data.erase(it);
}

int Chart::size() const {
    return data.size();
}

void Chart::replace(const int& pos, ChartData* newElement) {
    data.replace(pos, newElement);
}

void Chart::removeAt (const int& i) {
    delete data.at(i);
    data.removeAt(i);
}
