#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Model/model.h"
#include "View/mainwidget.h"

#include <QObject>

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
    void checkAllFilesSaved();
    void newChartData();
    void delChartData();
};

#endif
