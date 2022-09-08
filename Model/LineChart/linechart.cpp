#include "linechart.h"

#include "line.h"

LineChart::LineChart(const QString& path) : Chart(path) {}

LineChart::~LineChart() {}

bool LineChart::push_back(ChartData* x) {
    if (dynamic_cast<Line*>(x)) {
        data.push_back(x);
        return true;
    }
    return false;
}

bool LineChart::save() const {

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
        macroObj.insert("Type","LineChart");

        QJsonArray lines;
        for (auto i: data) {
            QJsonObject tmpObj;
            QJsonArray singleLine;
            for (auto it = static_cast<Line*>(i)->begin(); it != static_cast<Line*>(i)->end(); it++) {
                QJsonObject tmp;
                tmp.insert("X", (*it).first);
                tmp.insert("Y", (*it).second);
                singleLine.push_back(tmp);
            }
            tmpObj.insert("LineName", i->getName());
            tmpObj.insert("Points", singleLine);
            lines.push_back(tmpObj);
        }
        macroObj.insert("Lines", lines);
        file.write(QJsonDocument(macroObj).toJson());
        file.close();
        return true;
    }
    return false;
}

bool LineChart::load (const QString& _path) {

    QFile file(_path);
    if (!file.open(QFile::ReadOnly))
        return false;

    path = _path;

    /* CONTROLLO CHE CI SIA IL CAMPO TIPO E CHE SIA GIUSTO */
    QJsonObject obj = QJsonDocument().fromJson(file.readAll()).object();
    if (!obj.contains("Type") || obj["Type"] != "LineChart") {
        file.close();
        return false;
    }

    if (obj.contains("Lines") && obj["Lines"].isArray()) {
        data = QList<ChartData*>();
        QJsonArray lines = obj["Lines"].toArray();
        for (auto i: lines) {
            QJsonObject tmp = i.toObject();
            if (tmp.contains("LineName") && tmp["LineName"].isString() && tmp.contains("Points") && tmp["Points"].isArray()) {
                Line* line = new Line(tmp["LineName"].toString());
                for (auto j: tmp["Points"].toArray()) {
                    QJsonObject tmp2 = j.toObject();
                    if (tmp2.contains("X") && tmp2["X"].isDouble() && tmp2.contains("Y") && tmp2["Y"].isDouble())
                        line->push_back(QPair<double,double>(tmp2["X"].toDouble(),tmp2["Y"].toDouble()));
                }
                data.push_back(line);
            }
        }
    }

    file.close();
    return true;
}

bool LineChart::operator== (LineChart right) const {
    /* DEVO CONTROLLARE CHE NON CE NE SIA NESSUNO DI DIVERSO => TUTTI UGUALI */
    for (int i=0; i < size(); i++) {
        Line a = *static_cast<const Line*>(at(i));
        Line b = *static_cast<const Line*>(right.at(i));
        if (a != b)
            return false;
    }
    return true;
}

bool LineChart::operator!= (LineChart right) const {
    /* DEVO CONTROLLARE CHE CE NE SIA ALMENO UNO DI DIVERSO */
    for (int i=0; i< size(); i++) {
        Line a = *static_cast<const Line*>(at(i));
        Line b = *static_cast<const Line*>(right.at(i));
        if (a != b)
            return true;
    }
    return false;
}
