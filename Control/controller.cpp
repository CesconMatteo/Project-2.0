#include "controller.h"

Controller::Controller (QObject* parent) : QObject(parent) {}

void Controller::setup (Model* _model, MainWidget* _view) {
    model = _model;
    view = _view;
}

void Controller::openFile(QString dropPath) {
    QString path;
    if (dropPath.isEmpty())     // NON E' DRAG & DROP
        path = view->openFileDialog(model->getDesktopPath());
    else                        // E' DRAG & DROP
        path = dropPath;
    if (!path.isEmpty()) {      // CHECK SE NON E' DRAG & DROP E E' STATO INSERITO UN PATH CORRETTO
        if (model->loadChart(path))
            view->addNewTab(model->last());
        else
            view->errorDialog("File già aperto");
    }
}

void Controller::closeTab (int i) {

    /* CONTROLLO SE IL FILE E' SALVATO */
    if (!model->isSaved(i)) {
        SaveOptions opt = view->fileNotSavedDialog();
        switch (opt) {
            case Save:          /* SALVO */
                saveFile();
                break;
            case DontSave:      /* NON FACCIO NIENTE */
                break;
            case Cancel:        /* RITORNO IN MODO DA NON CHIUDERE IL TAB */
                return;
        }
    }

    /* RIMUOVO IL TAB */
    model->removeChart(i);
    view->closeTab(i);
}

void Controller::saveFile() {
    if (!view->tabsOpened()) // NON CI SONO TAB APERTE
        view->errorDialog("Nessun grafico aperto!");
    else {
        int index = view->currentTabIndex();
        if (model->chartFileExists(index)) {            // SE IL FILE GIA' ESISTE LO SALVO DIRETTAMENTE
            model->saveChart(index);
            view->okDialog("Salvataggio eseguito correttamente.");
        } else                                          // ALTRIMENTI ESEGUO SALVA COME
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

void Controller::checkAllFilesSaved() {
    if (!model->allSaved())         // SE NON SONO TUTTI SALVATI
        view->yesOrNoDialog("Non tutti i file sono stati salvati. Chiudere lo stesso?");
}
#include <iostream>
using namespace std;
void Controller::newChartData() {
    cout << "New";
}

void Controller::delChartData() {
    QString chartDataName = view->delChartDataDialog();
    if (chartDataName.isEmpty())                    // SCHIACCIO CANCEL SUL DIALOG
        return;
    if (model->removeChartData(view->currentTabIndex(), chartDataName))         // RIMOZIONE CORRETTA DAL MODEL
        if (view->delChartData(chartDataName))                                  // RIMOZIONE CORRETTA DAL VIEW
            view->okDialog("Rimozione corretta");
        else
            view->errorDialog("Rimozione errata");
    else
        view->errorDialog("Errore!");
}
