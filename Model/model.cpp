#include "model.h"

#include "BarChart/barchart.h"
#include "LineChart/linechart.h"
#include "PieChart/piechart.h"
#include "BarChart/barset.h"
#include "LineChart/line.h"
#include "PieChart/pieslice.h"

Model::Model () {
    desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

const QString& Model::getDesktopPath() const {
    return desktopPath;
}

const Chart* Model::chartAt (const int& i) const {
    return charts.at(i);
}

bool Model::loadChart (const QString& path) {
    Chart* x;

    /* CONTROLLO CHE IL FILE ESISTA E NON SIA CORROTTO */
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
        return false;
    QJsonObject obj = QJsonDocument().fromJson(file.readAll()).object();
    if (!obj.contains("Type")) {
        file.close();
        return false;
    }

    if (obj["Type"] == "PieChart")
        x = new PieChart();
    else if (obj["Type"] == "LineChart")
        x = new LineChart();
    else if (obj["Type"] == "BarChart")
        x = new BarChart();
    else
        return false;

    file.close();

    x->load(path);
    for (auto i: charts)
        if (i->getPath() == x->getPath()) {
            delete x;
            return false;
        }
    charts.push_back(x);
    saved.push_back(true);
    return true;
}

void Model::newChart (ChartType type) {

    Chart* x;
    switch (type) {
        case Bar:
            x = new BarChart("nuovo_grafico_barre");
            break;
        case Line:
            x = new LineChart("nuovo_grafico_linee");
            break;
        case Pie:
            x = new PieChart("nuovo_grafico_torta");
            break;
    }

    charts.push_back(x);
    saved.push_back(false);
}

bool Model::saveChart (const int& i) {
    bool ok = charts.at(i)->save();
    if (ok)
        saved[i] = true;
    return ok;
}

bool Model::saveAs (const int& i, const QString& newPath) {
    charts.at(i)->setPath(newPath);
    bool ok = charts.at(i)->save();
    if (ok)
        saved[i] = true;
    return ok;
}

bool Model::isSaved (const int& i) const {
    return saved.at(i);
}

bool Model::allSaved() const {
    for (auto i: saved)
        if (!i)
            return false;
    return true;
}

void Model::removeChart(const int& i) {
    charts.removeAt(i);
    saved.removeAt(i);
}

void Model::addChartData (const int& i, const QStringList& info) {          // INFO CONTIENE NEL PRIMO CAMPO IL NOME
                                                                            // PER BARCHART CONTIENE ANCHE I VALORI, IN CASO DI NON INSERIMENTO
    if (dynamic_cast<BarChart*>(charts.at(i))) {                            // CONTIENE 0
                                                                            // PER PIECHART CONTIENE ANCHE IL VALORE DELLA FETTA
        BarSet* newBarSet = new BarSet(info.at(0));
        for (int j=1; j < info.size(); j++)
            newBarSet->push_back(info.at(j).toDouble());
        charts.at(i)->push_back(newBarSet);

    } else if (dynamic_cast<LineChart*>(charts.at(i)))
        charts.at(i)->push_back(new class Line(info.at(0)));

    else if (dynamic_cast<PieChart*>(charts.at(i)))
        charts.at(i)->push_back(new PieSlice(info.at(0), info.at(1).toDouble()));

    saved[i] = false;
}

bool Model::removeChartData (const int& i, const QString& chartDataName) {
    for (auto it = charts.at(i)->begin(); it != charts.at(i)->end(); it++) {
        QString a = (*it)->getName();
        double s = static_cast<PieSlice*>(*it)->getValue();
        if ((*it)->getName() == chartDataName) {
            charts.at(i)->erase(it);
            saved[i] = false;
            return true;
        }
    }
    return false;
}

void Model::modifyChartData (const int& i, const QString& oldName, const QString& newName, const double& newValue) {
    bool done = false;
    if (dynamic_cast<PieChart*>(charts.at(i))) {

        for (auto it = charts.at(i)->begin(); it != charts.at(i)->end(); it++)
            if ((*it)->getName() == oldName) {
                (*it)->setName(newName);
                static_cast<PieSlice*>(*it)->setValue(newValue);
                done = true;
                break;
            }
    } else {
        for (auto it = charts.at(i)->begin(); it != charts.at(i)->end(); it++)
            if ((*it)->getName() == oldName) {
                (*it)->setName(newName);
                done = true;
                break;
            }
    }
    if (done)
        saved[i] = false;
}

Chart* Model::last() {
    return charts.last();
}

bool Model::chartFileExists(const int& i) const {
    return QFile().exists(charts.at(i)->getPath());
}
