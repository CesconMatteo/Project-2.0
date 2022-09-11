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

    unsigned int desktopWidth;
    unsigned int desktopHeight;

    QHBoxLayout* buttons();
    QPushButton* openFileBtn;
    QPushButton* newFileBtn;
    QPushButton* saveFileBtn;
    QPushButton* saveAsBtn;
    QPushButton* exportPDFBtn;
    bool eventFilter (QObject*, QEvent*);

    QTabWidget* tab;
    bool _tabsOpened;

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void setController (Controller*);
    void connectButtons();

    void addNewTab (Chart*);
    void dragEnterEvent (QDragEnterEvent*) final;
    void dropEvent (QDropEvent*) final;
    void closeTab (const int&);

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

    QStringList addChartData();
    void connectChartDataButtons();
    void connectSubButtons();
    QString delChartDataDialog();
    bool delChartData (const QString&);
    QPair<QString,QString> modChartData (const QString&);
    void exportPDF (const QString&);
    QPair<QString,QString> showChartDataOptionsMenu (QPushButton*);
    void showSubOptionsMenu (QPushButton*);
    QPair<QString,int> delPoint ();
    QList<QVariant> modSubChartData();

    QPair<double,double> addNewPoint (const QString&);
    QString newCategory();
    QString delCategory();

signals:
    QString fileDropped(QString);
    void closing(QCloseEvent*);
};

#endif
