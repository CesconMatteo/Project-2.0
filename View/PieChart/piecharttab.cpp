#include "piecharttab.h"

#include "Model/PieChart/pieslice.h"

PieChartTab::PieChartTab(Chart* chart, QWidget* parent) : ChartTab(parent) {
    QVBoxLayout* macroLayout = new QVBoxLayout();
    macroLayout->addLayout(header());

    if (chart->empty()) {
        macroLayout->addLayout(zeroDataTab("Inserire una fetta"));
        voidChart = true;
    } else {
        QHBoxLayout* horizontalLayout = new QHBoxLayout();
        setupScroll(chart);
        horizontalLayout->addWidget(scroll);
        horizontalLayout->addWidget(dxLayout(chart));
        updatePercentage();
        macroLayout->addLayout(horizontalLayout);
        voidChart = false;
    }

    setLayout(macroLayout);
}

QHBoxLayout* PieChartTab::header() {
    QHBoxLayout* layout = new QHBoxLayout();
    newChartDataBtn = new QPushButton("Nuova fetta");
    delChartDataBtn = new QPushButton("Elimina fetta");
    newChartDataBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    delChartDataBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    layout->addWidget(newChartDataBtn);
    layout->addWidget(delChartDataBtn);
    return layout;
}

void PieChartTab::setupScroll (Chart* chart) {

    QVBoxLayout* dataLayout = new QVBoxLayout();

    for (auto it = chart->begin(); it != chart->end(); it++) {

        QHBoxLayout* firstLayout = new QHBoxLayout();

        QLabel* sliceName = new QLabel((*it)->getName());            // ADD LABEL
        QFont font = sliceName->font();
        font.setBold(true);
        font.setPointSize(20);
        sliceName->setFont(font);
        firstLayout->addWidget(sliceName);
        chartDataNames.push_back(sliceName);

        /* BUTTON */
        QPushButton* btn = new QPushButton("Opzioni");
        firstLayout->addWidget(btn);
        QList<QPushButton*> tmp = QList<QPushButton*>();
        tmp.push_back(btn);
        subOptionButtons.push_back(tmp);

        QHBoxLayout* secondLayout = new QHBoxLayout();

        std::stringstream value;
        if (fmod(static_cast<PieSlice*>(*it)->getValue(), 1.0) == 0)
            value.precision(0);
        else
            value.precision(2);
        value << std::fixed;
        value << static_cast<PieSlice*>(*it)->getValue();
        QLineEdit* valueEdit = new QLineEdit(QString::fromStdString(value.str()));
        valueEdit->setAlignment(Qt::AlignCenter);
        secondLayout->addWidget(valueEdit);
        QList<QWidget*> tmpFC = QList<QWidget*>();
        tmpFC.push_back(valueEdit);
        firstColoumn.push_back(tmpFC);

        QLineEdit* percEdit = new QLineEdit();
        percEdit->setAlignment(Qt::AlignCenter);
        secondLayout->addWidget(percEdit);
        QList<QLineEdit*> tmpSC = QList<QLineEdit*>();
        tmpSC.push_back(percEdit);
        secondColoumn.push_back(tmpSC);

        QFrame* separator = createSeparator();
        chartDataSeparators.push_back(separator);

        QVBoxLayout* layout = new QVBoxLayout();
        layout->addLayout(firstLayout);
        layout->addLayout(secondLayout);

        dataLayout->addLayout(layout);
        dataLayout->addWidget(separator);
    }

    /* SET UP SCROLL */
    QWidget* scrollWidget = new QWidget();
    scrollWidget->setLayout(dataLayout);
    scroll = new QScrollArea();
    scroll->setWidget(scrollWidget);
}

QChartView* PieChartTab::dxLayout (Chart* chart) {

    QPieSeries* series = new QPieSeries();

    for (auto it = chart->begin(); it != chart->end(); it++) {
        QPieSlice* pSlice = new QPieSlice((*it)->getName(), static_cast<PieSlice*>(*it)->getValue());
        connect (pSlice, SIGNAL(hovered(bool)), this, SLOT(explodeSlice(bool)));
        series->append(pSlice);
    }

    for (auto i: series->slices())
        i->setLabel(i->label().append(" ").append(QString::number(i->percentage() * 100, 'f', 1)).append("%"));

    QChart* graphicChart = new QChart();
    graphicChart->addSeries(series);
    graphicChart->legend()->setAlignment(Qt::AlignRight);
    graphicChart->setAnimationOptions(QChart::AllAnimations);
    graphicChart->setAnimationDuration(2000);

    chartView = new QChartView(graphicChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

void PieChartTab::explodeSlice(bool what) {
    QList<QPieSlice*> slices = static_cast<QPieSeries*>(chartView->chart()->series().at(0))->slices();
    for (auto i: slices)
        if (static_cast<QPieSlice*>(QObject::sender()) == i) {
            i->setExploded(what);
            i->setLabelVisible(what);
        }
}

void PieChartTab::updatePercentage() {
    QList<QPieSlice*> slices = static_cast<QPieSeries*>(chartView->chart()->series().at(0))->slices();
    for (int i=0; i < secondColoumn.size(); i++) {

        std::stringstream value;
        if (fmod(slices.at(i)->percentage() * 100, 1.0) == 0)
            value.precision(0);
        else
            value.precision(1);
        value << std::fixed;
        value << slices.at(i)->percentage() * 100;
        secondColoumn.at(i).at(0)->setText(QString::fromStdString(value.str()));
    }

    for (auto i: static_cast<QPieSeries*>(chartView->chart()->series().at(0))->slices()) {
        QString str = QString::fromStdString(i->label().toStdString().substr(0, i->label().toStdString().find(' ')+1));
        i->setLabel(str.append(QString::number(i->percentage() * 100, 'f', 1)).append("%"));
    }

}

bool PieChartTab::delChartData (const QString& sliceName) {

    int j=0;
    int k=-1;

    for (auto i: static_cast<QPieSeries*>(chartView->chart()->series().at(0))->slices()){
        QString str = QString::fromStdString(i->label().toStdString().substr(0, i->label().toStdString().find(' ')));
        if (str == sliceName) {
            static_cast<QPieSeries*>(chartView->chart()->series().at(0))->remove(i);
            k=j;
            break;
        }
        j++;
    }

    if (k != -1) {

        /* RESIZE SCROLL */
        int y1 = chartDataNames.at(k)->geometry().topLeft().y();
        int y2 = firstColoumn.at(k).at(firstColoumn.at(k).size()-1)->geometry().bottomLeft().y();
        scroll->widget()->setFixedHeight(scroll->widget()->height()-(y2-y1+chartDataSeparators.at(k)->height()));

        for (int i=0; i < subOptionButtons.at(k).size(); i++) {
            delete firstColoumn.at(k).at(i);
            delete secondColoumn.at(k).at(i);
            delete subOptionButtons.at(k).at(i);
        }
        delete chartDataNames.at(k);
        delete chartDataSeparators.at(k);

        firstColoumn.removeAt(k);
        secondColoumn.removeAt(k);
        subOptionButtons.removeAt(k);
        chartDataNames.removeAt(k);
        chartDataSeparators.removeAt(k);

        updatePercentage();

        return true;
    }
    return false;
}
