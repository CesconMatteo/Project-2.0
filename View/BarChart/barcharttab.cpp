#include "barcharttab.h"

#include "Model/BarChart/barchart.h"
#include "Model/BarChart/barset.h"

BarChartTab::BarChartTab(Chart* chart, QWidget* parent) : ChartTab(parent) {
    QVBoxLayout* macroLayout = new QVBoxLayout();
    macroLayout->addLayout(header());

    if (chart->empty()) {
        macroLayout->addLayout(zeroDataTab("Inserire un set"));
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

QHBoxLayout* BarChartTab::header() {
    QHBoxLayout* layout = new QHBoxLayout();
    newChartDataBtn = new QPushButton("Nuovo set");
    delChartDataBtn = new QPushButton("Elimina set");
    QPushButton* newCat = new QPushButton("Nuova categoria");
    QPushButton* delCat = new QPushButton("Elimina categoria");
    newChartDataBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    delChartDataBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    newCat->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    delCat->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    layout->addWidget(newChartDataBtn);
    layout->addWidget(delChartDataBtn);
    layout->addWidget(newCat);
    layout->addWidget(delCat);
    return layout;
}

void BarChartTab::setupScroll (Chart* chart) {

    QVBoxLayout* dataLayout = new QVBoxLayout();

    firstColoumn = QList<QList<QWidget*>>();
    secondColoumn = QList<QList<QLineEdit*>>();

    for (auto it = chart->begin(); it != chart->end(); it++) {

        /* SET NAME LAYOUT */
        QHBoxLayout* setLayout = new QHBoxLayout();
        QLabel* setName = new QLabel((*it)->getName());                 // ADD LABEL
        QFont font = setName->font();
        font.setBold(true);
        font.setPointSize(20);
        setName->setFont(font);
        setLayout->addWidget(setName);
        chartDataNames.push_back(setName);

        QPushButton* setOptionBtn = new QPushButton("Opzioni");         // ADD BUTTON
        setLayout->addWidget(setOptionBtn);
        dataLayout->addLayout(setLayout);
        chartDataOptionButtons.push_back(setOptionBtn);

        QVBoxLayout* externalLayout = new QVBoxLayout();
        QList<QWidget*> tmpFC = QList<QWidget*>();
        QList<QLineEdit*> tmpSC = QList<QLineEdit*>();
        QList<QPushButton*> tmpBtn = QList<QPushButton*>();

        for (int i = 0; i < static_cast<BarChart*>(chart)->categories().size(); i++) {

            QHBoxLayout* internalLayout = new QHBoxLayout();

            /* CATEGORY */
            QLabel* label = new QLabel(static_cast<BarChart*>(chart)->categories().at(i));
            label->setAlignment(Qt::AlignCenter);
            label->setMinimumWidth(60);
            internalLayout->addWidget(label);
            tmpFC.push_back(label);

            /* VALUE */
            QLineEdit* edit;
            if (i < static_cast<BarSet*>(*it)->size()) {
                std::stringstream stream;
                if (fmod(static_cast<BarSet*>(*it)->at(i), 1) == 0)
                    stream.precision(0);
                else
                    stream.precision(2);
                stream << std::fixed;
                stream << static_cast<BarSet*>(*it)->at(i);
                edit = new QLineEdit (QString::fromStdString(stream.str()));
            } else
                edit = new QLineEdit("0");
            edit->setAlignment(Qt::AlignCenter);
            internalLayout->addWidget(edit);
            tmpSC.push_back(edit);

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

QChartView* BarChartTab::dxLayout (Chart* chart) {

    QStringList categories;
    for (auto& i: static_cast<BarChart*>(chart)->categories())
        categories.push_back(i);

    QBarSeries* series = new QBarSeries();
    for (auto it = chart->begin(); it != chart->end(); it++) {
        QBarSet* barSet = new QBarSet((*it)->getName());
        for (auto it2 = static_cast<BarSet*>(*it)->begin(); it2 != static_cast<BarSet*>(*it)->end(); it2++) {
            barSet->append((*it2));                 // CONTROLLARE PER 0
        }
        series->append(barSet);
    }


    QChart* graphicChart = new QChart();
    graphicChart->addSeries(series);
    graphicChart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    graphicChart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    graphicChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    graphicChart->legend()->setVisible(true);
    graphicChart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(graphicChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

bool BarChartTab::delChartData (const QString& barsetName) {

    int j=0;            // INDICI PER VERIFICARE CHE CI SIA LO SPECIFICO BARSET
    int k=-1;

    /* RIMOZIONE DAL GRAFICO */
    QBarSeries* series = new QBarSeries();
    QList<QBarSet*> barsets = (static_cast<QBarSeries*>(chartView->chart()->series().at(0))->barSets());
    for (auto it = barsets.begin(); it != barsets.end(); it++) {
        if ((*it)->label() != barsetName)
            series->append(*it);
        else {
            k = j;
            break;
        }
        j++;
    }
    chartView->chart()->removeAllSeries();
    chartView->chart()->addSeries(series);

    /* RIMOZIONE DAI DATI */
    if (k != -1) {

        /* RESIZE SCROLL */
        scroll->widget()->setFixedHeight((scroll->widget()->height() / j) * (j-1));

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
