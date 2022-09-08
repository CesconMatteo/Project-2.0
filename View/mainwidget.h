#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "Model/chart.h"

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTabWidget>

class Controller;

enum SaveOptions { Save, DontSave, Cancel };

class MainWidget : public QWidget {
    Q_OBJECT
private:
    Controller* controller;

    /* DESKTOP DIMENSIONS */
    unsigned int desktopWidth;
    unsigned int desktopHeight;

    /* BUTTONS RELATED ITEMS */
    QHBoxLayout* buttons();
    QPushButton* openFileBtn;
    QPushButton* newFileBtn;
    QPushButton* saveFileBtn;
    QPushButton* saveAsBtn;
    QPushButton* exportPDFBtn;
    bool eventFilter (QObject*, QEvent*);

    /* TAB RELATED ITEMS */
    QTabWidget* tab;
    bool _tabsOpened;

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void setController (Controller*);
    void connectButtons();

    /* TAB */
    void addNewTab (Chart*);
    void dragEnterEvent (QDragEnterEvent*) final;
    void dropEvent (QDropEvent*) final;
    void closeTab (const int&);

    /* DIALOGS */
    QString openFileDialog (const QString&);
    void okDialog (const QString&);
    void errorDialog (const QString&);
    SaveOptions fileNotSavedDialog();
    QString savePathDialog(const QString&);
    bool yesOrNoDialog (const QString&);
    QString chooseTypeDialog();

    bool tabsOpened() const;
    int currentTabIndex() const;
    void updatePath (const QString&);
    void closeEvent (QCloseEvent*);

    /* RIMBALZO CHIAMATE */
    QString delChartDataDialog();
    bool delChartData (const QString&);

signals:
    QString fileDropped(QString);
    void closing();
};

#endif
