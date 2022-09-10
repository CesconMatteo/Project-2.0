#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Model/model.h"
#include "View/mainwidget.h"

#include <QObject>
#include <QCloseEvent>

class Controller : public QObject {
    Q_OBJECT
private:
    Model* model;
    MainWidget* view;
public:
    Controller (QObject* =nullptr);
    void setup (Model*, MainWidget*);
public slots:
    void openFile (QString =0);
    void saveFile();
    void saveAs();
    void newFile();
    void closeTab (int);
    void checkAllFilesSaved(QCloseEvent*);
    void newChartData();
    void chartDataOptions();
    void delChartData(QString =0);
    void modChartData (const QString&);
    void exportPDF();
};

#endif
