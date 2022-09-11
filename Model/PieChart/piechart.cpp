#include "piechart.h"

#include "pieslice.h"

PieChart::PieChart(const QString& _path) : Chart(_path) {}

PieChart::~PieChart() {}

bool PieChart::push_back (ChartData* x) {
    if (dynamic_cast<PieSlice*>(x)){
        data.push_back(x);
        return true;
    }
    return false;
}

bool PieChart::save() const {

    if (QFile::exists(path))
        QFile::remove(path);

    int lastSlash = path.toStdString().find_last_of("/");
    QString dirPath = QString::fromStdString(path.toStdString().substr(0, lastSlash));
    if (!QDir(dirPath).exists())
        return false;

    QFile file(path);
    if (file.open(QFile::ReadWrite)) {
        QJsonObject macroObj;
        macroObj.insert("Type", "PieChart");
        QJsonArray values;
        for (auto it = cbegin(); it != cend(); it++) {
            QJsonObject tmp;
            tmp.insert("Name", (*it)->getName());
            tmp.insert("Value", static_cast<PieSlice*>(*it)->getValue());
            values.push_back(tmp);
        }
        macroObj.insert("Values", values);
        file.write(QJsonDocument(macroObj).toJson());
        file.close();
        return true;
    }
    return false;
}

bool PieChart::load (const QString& _path) {

    QFile file(_path);
    if (!file.open(QFile::ReadOnly))
        return false;

    path = _path;

    QJsonObject obj = QJsonDocument().fromJson(file.readAll()).object();
    if (!obj.contains("Type") || obj["Type"] != "PieChart") {
        file.close();
        return false;
    }

    if (obj.contains("Values") && obj["Values"].isArray()) {
        data = QList<ChartData*>();
        QJsonArray values = obj["Values"].toArray();
        for (auto i: values) {
            QJsonObject tmp = i.toObject();
            if (tmp.contains("Name") && tmp["Name"].isString() && tmp.contains("Value") && tmp["Value"].isDouble())
                data.push_back(new PieSlice (tmp["Name"].toString(), tmp["Value"].toDouble()));
        }
    }

    file.close();
    return true;
}

bool PieChart::operator== (PieChart right) const {
    for (int i=0; i< data.size(); i++) {
        PieSlice a = *static_cast<const PieSlice*>(at(i));
        PieSlice b = *static_cast<const PieSlice*>(right.at(i));
        if (a != b)
            return false;
    }
    return true;
}

bool PieChart::operator!= (PieChart right) const {
    for (int i=0; i < size(); i++) {
        PieSlice a = *static_cast<const PieSlice*>(at(i));
        PieSlice b = *static_cast<const PieSlice*>(right.at(i));
        if (a != b)
            return true;
    }
    return false;
}
