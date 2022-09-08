#include "barchart.h"

#include "barset.h"

BarChart::BarChart(const QString& path) : Chart(path) {
    _categories = QStringList();
}

BarChart::~BarChart() {}

bool BarChart::push_back(ChartData* x) {
    if (dynamic_cast<BarSet*>(x)) {
        data.push_back(x);
        return true;
    }
    return false;
}

bool BarChart::save() const {

    /* SE ESISTE GIA' IL FILE LO RIMUOVO, LO RICREERO' SALVANDO */
    if (QFile::exists(path))
        QFile::remove(path);

    /* CONTROLLO SE LA CARTELLA SPECIFICATA ESISTE */
    int lastSlash = path.toStdString().find_last_of("/");
    QString dirPath = QString::fromStdString(path.toStdString().substr(0, lastSlash));
    if (!QDir(dirPath).exists())
        QDir().mkdir(dirPath);

    QFile file(path);
    if (file.open(QFile::ReadWrite)) {

        QJsonObject macroObj;
        macroObj.insert("Type","BarChart");

        QJsonArray catArray;
        for (auto i: _categories)
            catArray.push_back(i);
        macroObj.insert("Categories", catArray);

        QJsonArray valuesArray;
        for (auto i: data) {
            QJsonObject tmp;
            tmp.insert("Name", i->getName());
            QJsonArray set;
            for (auto it = static_cast<BarSet*>(i)->begin(); it != static_cast<BarSet*>(i)->end(); it++)
                set.push_back(*it);
            tmp.insert("Values", set);
            valuesArray.push_back(tmp);
        }
        macroObj.insert("Sets", valuesArray);
        file.write(QJsonDocument(macroObj).toJson());
        file.close();
        return true;
    }
    return false;
}

bool BarChart::load(const QString& _path) {
    QFile file(_path);
    if (!file.open(QFile::ReadOnly))
        return false;
    path = _path;
    QJsonDocument doc = QJsonDocument().fromJson(file.readAll());
    QJsonObject obj = doc.object();
    if (!obj.contains("Type") || obj["Type"] != "BarChart")
        return false;
    if (obj.contains("Categories") && obj["Categories"].isArray()) {
        QJsonArray cat = obj["Categories"].toArray();
        _categories = QStringList();
        for (auto i: cat)
            _categories.push_back(i.toString());
    }
    if  (obj.contains("Sets") && obj["Sets"].isArray()) {
        data = QList<ChartData*>();
        for (auto i: obj["Sets"].toArray()) {
            QJsonObject singleBarSet = i.toObject();
            if (singleBarSet.contains("Name") && singleBarSet["Name"].isString() && singleBarSet.contains("Values") && singleBarSet["Values"].isArray()) {
                BarSet* barSet = new BarSet(singleBarSet["Name"].toString());
                for (unsigned int j=0; j < _categories.size(); j++) {
                    if (j < singleBarSet["Values"].toArray().size())
                        barSet->push_back(singleBarSet["Values"].toArray().at(j).toDouble());
                    else
                        barSet->push_back(0);
                }
                data.push_back(barSet);
            }
        }
    }
    file.close();
    return true;
}

bool BarChart::operator== (BarChart right) const {
    for (int i=0; i < size(); i++) {
        BarSet a = *static_cast<const BarSet*>(at(i));
        BarSet b = *static_cast<const BarSet*>(right.at(i));
        if (a != b)
            return false;
    }
    return true;
}

bool BarChart::operator!= (BarChart right) const {
    for (int i=0; i < size(); i++) {
        BarSet a = *static_cast<const BarSet*>(at(i));
        BarSet b = *static_cast<const BarSet*>(right.at(i));
        if (a != b)
            return true;
    }
    return false;
}

double BarChart::maxValue() const {
    double max = std::numeric_limits<double>::min();
    for (auto i: data)
        if (static_cast<BarSet*>(i)->maxValue() > max)
            max = static_cast<BarSet*>(i)->maxValue();
    if (max != std::numeric_limits<double>::min())
        return max;
    else
        return 0;
}

double BarChart::minValue() const {
    double min = std::numeric_limits<double>::max();
    for (auto i: data)
        if (static_cast<BarSet*>(i)->minValue() < min)
            min = static_cast<BarSet*>(i)->minValue();
    if (min != std::numeric_limits<double>::max())
        return min;
    else
        return 0;
}

const QStringList& BarChart::categories() const {
    return _categories;
}

void BarChart::addCategory (QString x) {
    _categories.push_back(x);
}

void BarChart::removeCategory (QStringList::ConstIterator it) {
    _categories.erase(it);
}
