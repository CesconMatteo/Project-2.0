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

    QString delChartDataDialog();
    virtual bool delChartData (const QString&) =0;

protected:

    QPushButton* newChartDataBtn;
    QPushButton* delChartDataBtn;

    QList<QLabel*> chartDataNames;
    QList<QPushButton*> chartDataOptionButtons;
    QList<QFrame*> chartDataSeparators;

    QChartView* chartView;
    QScrollArea* scroll;

    QList<QList<QWidget*>> firstColoumn;        // SUBCHARTDATA FIRST COLOUMN
    QList<QList<QLineEdit*>> secondColoumn;     // SUBCHARTDATA SECOND COLOUMN
    QList<QList<QPushButton*>> subOptionButtons;   // SUBCHARTDATA OPTION BUTTONS

    bool voidChart;


    QFrame* createSeparator();
    static QHBoxLayout* zeroDataTab (const QString&);
private:
    void resizeEvent (QResizeEvent*) final;
};

#endif
