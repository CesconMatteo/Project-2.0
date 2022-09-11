#ifndef CHARTAB_H
#define CHARTAB_H

#include <QWidget>
#include <QFrame>
#include <QtCharts/QtCharts>

class ChartTab: public QWidget {
public:
    ChartTab (QWidget* =nullptr);

    QPushButton* getNewChartDataBtn() const;
    QPushButton* getDelChartDataBtn() const;

    virtual QPair<QStringList,bool> addChartDataDialog() =0;
    virtual void addChartData (const QStringList&) =0;
    QString delChartDataDialog();
    virtual bool delChartData (const QString&) =0;
    virtual QPair<QString,QString> showChartDataOptions (QPushButton*);
    virtual QPair<QString,QString> modChartData (const QString&) =0;
    virtual QPair<QAction*,QAction*> showSubOptions (QPushButton*);
    virtual QList<QVariant> modSubChartData();

    void exportPDF (const QString&);

    const QList<QPushButton*>& getChartDataOptionButtons() const;
    const QList<QList<QPushButton*>>& getSubOptionButtons() const;

    void execMenu();

protected:

    QPushButton* newChartDataBtn;
    QPushButton* delChartDataBtn;

    QList<QLabel*> chartDataNames;
    QList<QPushButton*> chartDataOptionButtons;
    QList<QFrame*> chartDataSeparators;
    QList<QVBoxLayout*> chartDataLayouts;

    QChartView* chartView;
    QScrollArea* scroll;

    QList<QList<QWidget*>> firstColoumn;        // SUBCHARTDATA FIRST COLOUMN
    QList<QList<QWidget*>> secondColoumn;     // SUBCHARTDATA SECOND COLOUMN
    QList<QList<QPushButton*>> subOptionButtons;   // SUBCHARTDATA OPTION BUTTONS

    bool voidChart;

    QMenu* menu;
    QPair<int,int> buttonIndexes;

    QFrame* createSeparator();
    void zeroDataTab (const QString&);
    QLabel* zeroDataLabel;
};

#endif
