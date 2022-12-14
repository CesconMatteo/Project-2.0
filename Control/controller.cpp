#include "controller.h"

Controller::Controller (QObject* parent) : QObject(parent) {}

void Controller::setup (Model* _model, MainWidget* _view) {
    model = _model;
    view = _view;
}

void Controller::openFile(QString dropPath) {
    QString path;
    if (dropPath.isEmpty())
        path = view->openFileDialog(model->getDesktopPath());
    else
        path = dropPath;
    if (!path.isEmpty()) {
        if (model->loadChart(path))
            view->addNewTab(model->last());
        else
            view->errorDialog("File già aperto");
    }
}

void Controller::closeTab (int i) {

    if (!model->isSaved(i)) {
        SaveOptions opt = view->fileNotSavedDialog();
        switch (opt) {
            case Save:
                saveFile();
                break;
            case DontSave:
                break;
            case Cancel:
                return;
        }
    }

    /* RIMUOVO IL TAB */
    model->removeChart(i);
    view->closeTab(i);
}

void Controller::saveFile() {
    if (!view->tabsOpened())
        view->errorDialog("Nessun grafico aperto!");
    else {
        int index = view->currentTabIndex();
        if (model->chartFileExists(index)) {
            model->saveChart(index);
            view->okDialog("Salvataggio eseguito correttamente.");
        } else
            saveAs();
    }
}

void Controller::saveAs() {
    if (!view->tabsOpened())
        view->errorDialog("Nessun grafico aperto!");
    else {
        QString path = view->savePathDialog(model->getDesktopPath());
        if (path.isEmpty())
            view->errorDialog("Percorso non corretto!");
        else {
            model->saveAs(view->currentTabIndex(), path);
            view->updatePath(path);
            view->okDialog("Salvataggio eseguito correttamente.");
        }
    }
}

void Controller::newFile() {
    QString path = view->chooseTypeDialog();
    if (path == "Grafico a barre")
        model->newChart(ChartType::Bar);
    else if (path == "Grafico con linee")
        model->newChart(ChartType::Line);
    else if (path == "Grafico a torta")
        model->newChart(ChartType::Pie);
    else
        return;
    view->addNewTab(model->last());
}

void Controller::checkAllFilesSaved (QCloseEvent* e) {
    if (!model->allSaved())
        if (!view->yesOrNoDialog("Non tutti i file sono stati salvati. Chiudere lo stesso?"))
            e->ignore();
}

void Controller::newChartData() {
    QStringList info = view->addChartData();
    if (info.isEmpty())
        return;
    model->addChartData(view->currentTabIndex(), info);
}

void Controller::delChartData (QString chartDataName) {
    if (chartDataName.isEmpty()) {
        chartDataName = view->delChartDataDialog();
        if (chartDataName.isEmpty())
            return;
    }
    if (model->removeChartData(view->currentTabIndex(), chartDataName))
        if (view->delChartData(chartDataName))
            view->okDialog("Rimozione corretta");
        else
            view->errorDialog("Errore!");
    else
        view->errorDialog("Errore! Elemento non presente nel file");
}

void Controller::chartDataOptions() {
    QPair<QString,QString> info = view->showChartDataOptionsMenu(static_cast<QPushButton*>(QObject::sender()));
    if (info.first == "Modifica") {
        QPair<QString,QString> newData = view->modChartData(info.second);
        model->modifyChartData(view->currentTabIndex(), info.second, newData.first, newData.second.toDouble());
    } else if (info.first == "Nuovo punto") {
        model->addPoint(view->currentTabIndex(), info.second, view->addNewPoint(info.second));
    } else if (info.first == "Elimina")
        delChartData(info.second);
}

void Controller::newCategory() {
    QString newCat = view->newCategory();
    if (!newCat.isEmpty())
        model->addCategory(view->currentTabIndex(),newCat);
}

void Controller::delCategory() {
    QString cat = view->delCategory();
    if (!cat.isEmpty())
        model->removeCategory(view->currentTabIndex(),cat);
}

void Controller::subOptions() {
    view->showSubOptionsMenu(static_cast<QPushButton*>(QObject::sender()));

}

void Controller::delPoint() {
    QPair<QString,int> info = view->delPoint();
    model->removePoint(view->currentTabIndex(), info.first, info.second);
}

void Controller::modSubChartData() {
    QList<QVariant> info = view->modSubChartData();
    if (info.size() == 3)
        model->modSubChartData(view->currentTabIndex(), info.at(1).toString(), info.at(2).toInt(), QPair<double,double>(info.at(0).toDouble(),0));
    else if (info.size() == 4)
        model->modSubChartData(view->currentTabIndex(), info.at(2).toString(), info.at(3).toInt(), QPair<double,double>(info.at(0).toDouble(),info.at(1).toDouble()));
}

void Controller::exportPDF() {
    view->exportPDF(model->getDesktopPath());
}
