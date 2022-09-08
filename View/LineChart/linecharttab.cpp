#include "linecharttab.h"

#include "Model/LineChart/line.h"

LineChartTab::LineChartTab(Chart* chart, QWidget* parent) : ChartTab(parent) {
    QVBoxLayout* macroLayout = new QVBoxLayout();
    macroLayout->addLayout(header());

    if (chart->empty()) {
        macroLayout->addLayout(zeroDataTab("Inserire una linea"));
        voidChart = true;
    } else {
        QHBoxLayout* horizontalLayout = new QHBoxLayout();
        setupScroll(chart);
        horizontalLayout->addWidget(scroll);
        horizontalLayout->addWidget(dxLayout(chart));
        macroLayout->addLayout(horizontalLayout);
        voidChart = false;
    }

    setLayout(macroLayout);
}

QHBoxLayout* LineChartTab::header() {
    QHBoxLayout* layout = new QHBoxLayout();
    newChartDataBtn = new QPushButton("Nuova linea");
    delChartDataBtn = new QPushButton("Elimina linea");
    newChartDataBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    delChartDataBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    layout->addWidget(newChartDataBtn);
    layout->addWidget(delChartDataBtn);
    return layout;
}

void LineChartTab::setupScroll (Chart* chart) {

    QVBoxLayout* dataLayout = new QVBoxLayout();

    for (auto it = chart->begin(); it != chart->end(); it++) {

        /* LINE NAME LAYOUT */
        QHBoxLayout* lineNameLayout = new QHBoxLayout();
        QLabel* label = new QLabel((*it)->getName());
        QFont font = label->font();
        font.setBold(true);
        font.setPointSize(20);
        label->setFont(font);
        lineNameLayout->addWidget(label);
        chartDataNames.push_back(label);

        QPushButton* setOptionBtn = new QPushButton("Opzioni");         // ADD BUTTON
        lineNameLayout->addWidget(setOptionBtn);
        dataLayout->addLayout(lineNameLayout);
        chartDataOptionButtons.push_back(setOptionBtn);

        QVBoxLayout* externalLayout = new QVBoxLayout();
        QList<QWidget*> tmpFC = QList<QWidget*>();
        QList<QLineEdit*> tmpSC = QList<QLineEdit*>();
        QList<QPushButton*> tmpBtn = QList<QPushButton*>();

        for (auto it2 = static_cast<Line*>(*it)->begin(); it2 != static_cast<Line*>(*it)->end(); it2++) {
            QHBoxLayout* internalLayout = new QHBoxLayout();

            /* LINEEDIT X */
            stringstream x;
            if (fmod((*it2).first, 1.0) == 0)       // CHECK FOR SET THE PRECISION
                x.precision(0);
            else
                x.precision(2);
            x << std::fixed;
            x << (*it2).first;
            QLineEdit* xEdit = new QLineEdit(QString::fromStdString(x.str()));
            xEdit->setAlignment(Qt::AlignCenter);
            internalLayout->addWidget(xEdit);
            tmpFC.push_back(xEdit);

            /* LINEEDIT Y */
            stringstream y;
            if (fmod((*it2).second, 1.0) == 0)       // CHECK FOR SET THE PRECISION
                y.precision(0);
            else
                y.precision(2);
            y << std::fixed;
            y << (*it2).second;
            QLineEdit* yEdit = new QLineEdit(QString::fromStdString(y.str()));
            yEdit->setAlignment(Qt::AlignCenter);
            internalLayout->addWidget(yEdit);
            tmpSC.push_back(yEdit);

            /* BUTTON */
            QPushButton* btn = new QPushButton("···");
            btn->setFixedWidth(25);
            internalLayout->addWidget(btn);
            tmpBtn.push_back(btn);

            externalLayout->addLayout(internalLayout);
        }

        firstColoumn.push_back(tmpFC);
        secondColoumn.push_back(tmpSC);
        subOptionButtons.push_back(tmpBtn);

        QFrame* separator = createSeparator();
        chartDataSeparators.push_back(separator);

        dataLayout->addLayout(externalLayout);
        dataLayout->addWidget(separator);
    }

    /* SET UP SCROLL */
    QWidget* scrollWidget = new QWidget();
    scrollWidget->setLayout(dataLayout);
    scroll = new QScrollArea();
    scroll->setWidget(scrollWidget);
}

QChartView* LineChartTab::dxLayout (Chart* chart) {

    QChart* graphicChart = new QChart();
    graphicChart->setAnimationOptions(QChart::SeriesAnimations);
    graphicChart->setAnimationOptions(QChart::GridAxisAnimations);
    graphicChart->setAnimationDuration(5000);

    for (auto it = chart->begin(); it != chart->end(); it++) {

        QLineSeries* line = new QLineSeries();
        line->setName((*it)->getName());            // LINE NAME

        /* APPEND ALL POINTS */
        for (auto it2 = static_cast<Line*>(*it)->begin(); it2 != static_cast<Line*>(*it)->end(); it2++)
            line->append((*it2).first, (*it2).second);
        graphicChart->addSeries(line);              // ADD LINE
    }

    /* GRAPHIC OPTIONS */
    graphicChart->createDefaultAxes();
    graphicChart->axes(Qt::Orientation::Horizontal).at(0)->setTitleText("X");
    graphicChart->axes(Qt::Orientation::Vertical).at(0)->setTitleText("Y");
    chartView = new QChartView(graphicChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}
#include <iostream>
using namespace std;
bool LineChartTab::delChartData (const QString& lineName) {

    int j=0;
    int k=-1;

    for (auto i: chartView->chart()->series()) {
        if (i->name() == lineName) {
            delete i;
            k = j;
            break;
        }
        j++;
    }

    /* RIMOZIONE DAI DATI */
    if (k != -1) {

        /* RESIZE SCROLL */
        int y1 = chartDataNames.at(k)->geometry().topLeft().y();
        int y2 = firstColoumn.at(k).at(firstColoumn.at(k).size()-1)->geometry().bottomLeft().y();
        scroll->widget()->setFixedHeight(scroll->widget()->height()-(y2-y1));

        for (int i=0; i < subOptionButtons.at(k).size(); i++) {
            delete firstColoumn.at(k).at(i);
            delete secondColoumn.at(k).at(i);
            delete subOptionButtons.at(k).at(i);
        }
        delete chartDataOptionButtons.at(k);
        delete chartDataNames.at(k);

        firstColoumn.removeAt(k);
        secondColoumn.removeAt(k);
        subOptionButtons.removeAt(k);
        chartDataOptionButtons.removeAt(k);
        chartDataNames.removeAt(k);

        return true;
    }
    return false;
}
