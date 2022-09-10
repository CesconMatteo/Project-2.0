#include "barcharttab.h"

#include "Model/BarChart/barchart.h"
#include "Model/BarChart/barset.h"

BarChartTab::BarChartTab(Chart* chart, QWidget* parent) : ChartTab(parent) {
    QVBoxLayout* macroLayout = new QVBoxLayout();
    macroLayout->addLayout(header());

    if (chart->empty()) {
        zeroDataTab("Inserire un set");
        macroLayout->addWidget(zeroDataLabel);
        voidChart = true;
    } else {
        QHBoxLayout* horizontalLayout = new QHBoxLayout();
        setupScroll(chart);
        horizontalLayout->addWidget(scroll);
        dxLayout(chart);
        horizontalLayout->addWidget(chartView);
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
        setName->setAlignment(Qt::AlignCenter);
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

    dataLayout->setSizeConstraint(QLayout::SetFixedSize);

    /* SET UP SCROLL */
    QWidget* scrollWidget = new QWidget();
    scrollWidget->setLayout(dataLayout);
    scroll = new QScrollArea();
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll->setWidget(scrollWidget);
}

void BarChartTab::dxLayout (Chart* chart) {

    categories = QStringList();
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
    axisY->setTickCount(10);
    series->attachAxis(axisY);

    graphicChart->legend()->setVisible(true);
    graphicChart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(graphicChart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

void BarChartTab::resizeAxis() {
    double max = std::numeric_limits<double>::min();
    double min = std::numeric_limits<double>::max();
    for (auto& i: secondColoumn) {
        for (auto j: i) {
            if (j->text().toDouble() > max)
                max = j->text().toDouble();
            if (j->text().toDouble() < min)
                min = j->text().toDouble();
        }
    }
    static_cast<QValueAxis*>(chartView->chart()->axes(Qt::Vertical).at(0))->setRange(min,max);
}

QStringList BarChartTab::addChartDataDialog() {
    QDialog dialogWindow(this);
    dialogWindow.setWindowTitle("Nuovo set");
    QFormLayout* layout = new QFormLayout();
    QLineEdit* setName = new QLineEdit();
    QLabel* label = new QLabel("Nome set");
    QFont font = label->font();
    font.setBold(true);
    font.setPointSize(20);
    label->setFont(font);
    layout->addRow(label, setName);
    QList<QLineEdit*> edits;
    for (int i=0; i < categories.size(); i++) {                 // CICLA TANTE VOLTE QUANTE LE CATEGORIE
        QLineEdit* tmp = new QLineEdit();
        tmp->setValidator(new QRegularExpressionValidator(QRegularExpression("[+-]?([0-9]*[.])?[0-9]+")));
        layout->addRow(categories.at(i), tmp);
        edits.push_back(tmp);
    }
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialogWindow);
    connect(&buttonBox, SIGNAL(accepted()), &dialogWindow, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialogWindow, SLOT(reject()));
    layout->addRow(&buttonBox);
    dialogWindow.setLayout(layout);
    setName->setFocus();
    QStringList results = QStringList();
    if (dialogWindow.exec() == QDialog::Accepted) {
        if (setName->text().isEmpty())
            return results;
        results.push_back(setName->text());
        for (auto i: edits)
            if (!i->text().isEmpty())
                results.push_back(i->text());
            else
                results.push_back("0");
    }
    return results;
}

void BarChartTab::addChartData(const QStringList& info) {

    if (!voidChart) {

        QBarSet* barset = new QBarSet(info.at(0));
        for (int i=1; i < info.size(); i++)
            barset->append(info.at(i).toDouble());

        QBarSeries* newBarSeries = new QBarSeries();
        QList<QBarSet*> barsets;
            barsets = static_cast<QBarSeries*>(chartView->chart()->series().at(0))->barSets();
            for (auto it = barsets.begin(); it != barsets.end(); it++)
                newBarSeries->append(*it);

        newBarSeries->append(barset);

        QHBoxLayout* setLayout = new QHBoxLayout();
        QLabel* setName = new QLabel(info.at(0));
        setLayout->addWidget(setName);
        QFont font = setName->font();
        font.setBold(true);
        font.setPointSize(20);
        setName->setAlignment(Qt::AlignCenter);
        setName->setFont(font);
        setLayout->addWidget(setName);
        chartDataNames.push_back(setName);

        QPushButton* setOptionBtn = new QPushButton("Opzioni");         // ADD BUTTON
        setLayout->addWidget(setOptionBtn);
        chartDataOptionButtons.push_back(setOptionBtn);

        QVBoxLayout* externalLayout = new QVBoxLayout();
        externalLayout->addLayout(setLayout);
        QList<QWidget*> tmpFC = QList<QWidget*>();
        QList<QLineEdit*> tmpSC = QList<QLineEdit*>();
        QList<QPushButton*> tmpBtn = QList<QPushButton*>();

        for (int i=0; i < categories.size(); i++) {

            QHBoxLayout* internalLayout = new QHBoxLayout();

            QLabel* label = new QLabel(categories.at(i));
            label->setAlignment(Qt::AlignCenter);
            internalLayout->addWidget(label);
            tmpFC.push_back(label);

            /* VALUE */
            QLineEdit* edit;
            std::stringstream stream;
            if (fmod(info.at(i+1).toDouble(), 1) == 0)
                stream.precision(0);
            else
                stream.precision(2);
            stream << std::fixed;
            stream << info.at(i+1).toDouble();
            edit = new QLineEdit (QString::fromStdString(stream.str()));
            edit->setAlignment(Qt::AlignCenter);
            internalLayout->addWidget(edit);
            tmpSC.push_back(edit);

            /* BUTTON */
            QPushButton* btn = new QPushButton("···");
            internalLayout->addWidget(btn);
            tmpBtn.push_back(btn);

            externalLayout->addLayout(internalLayout);

        }

        QFrame* separator = createSeparator();
        chartDataSeparators.push_back(separator);

        firstColoumn.push_back(tmpFC);
        secondColoumn.push_back(tmpSC);
        subOptionButtons.push_back(tmpBtn);

        chartView->chart()->removeAllSeries();
        chartView->chart()->addSeries(newBarSeries);
        static_cast<QVBoxLayout*>(scroll->widget()->layout())->addLayout(externalLayout);
        static_cast<QVBoxLayout*>(scroll->widget()->layout())->addWidget(separator);

        resizeAxis();


    } else {

        BarChart* x = new BarChart();
        for (auto& i: categories)
            x->addCategory(i);
        BarSet* y = new BarSet(info.at(0));
        for (int i=0; i < categories.size(); i++)
            y->push_back(info.at(i+1).toDouble());
        x->push_back(y);

        QHBoxLayout* horizontalLayout = new QHBoxLayout();

        /* SCROLL */
        setupScroll(x);
        horizontalLayout->addWidget(scroll);

        /* RICREO IL GRAFICO */
        dxLayout(x);
        horizontalLayout->addWidget(chartView);

        delete zeroDataLabel;
        zeroDataLabel = nullptr;
        static_cast<QVBoxLayout*>(layout())->addLayout(horizontalLayout);

        delete x;
        voidChart = false;
    }
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
        else
            k = j;
        j++;
    }
    chartView->chart()->removeAllSeries();
    chartView->chart()->addSeries(series);

    /* RIMOZIONE DAI DATI */
    if (k != -1) {

        for (int i=0; i < subOptionButtons.at(k).size(); i++) {
            delete firstColoumn.at(k).at(i);
            delete secondColoumn.at(k).at(i);
            delete subOptionButtons.at(k).at(i);
        }
        delete chartDataOptionButtons.at(k);
        delete chartDataNames.at(k);
        delete chartDataSeparators.at(k);

        firstColoumn.removeAt(k);
        secondColoumn.removeAt(k);
        subOptionButtons.removeAt(k);
        chartDataOptionButtons.removeAt(k);
        chartDataNames.removeAt(k);
        chartDataSeparators.removeAt(k);

        /* CONTROLLO SE E' VUOTO */
        if (firstColoumn.size() == 0) {
            voidChart = true;
            delete scroll;
            delete chartView;

            delete layout()->takeAt(1);
            zeroDataTab("Inserire un set");
            static_cast<QVBoxLayout*>(layout())->addWidget(zeroDataLabel);
        }
        if (!voidChart)
            resizeAxis();
        return true;
    }

    return false;
}

QPair<QString, QString> BarChartTab::modChartData (const QString& chartDataName) {

    QDialog dialogWindow(this);
    dialogWindow.setWindowTitle("Modifica nome set");

    QFormLayout* layout = new QFormLayout();
    QLineEdit* newChartDataName = new QLineEdit(chartDataName);
    layout->addRow("Nome set", newChartDataName);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialogWindow);
    connect(&buttonBox, SIGNAL(accepted()), &dialogWindow, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialogWindow, SLOT(reject()));
    layout->addRow(&buttonBox);
    dialogWindow.setLayout(layout);
    newChartDataName->setFocus();

    if (!(dialogWindow.exec() == QDialog::Accepted))
        return QPair<QString,QString>();
    if (newChartDataName->text().isEmpty() || newChartDataName->text() == chartDataName)
        return QPair<QString,QString>();

    for (auto i: chartDataNames)
        if (i->text() == chartDataName)
            i->setText(newChartDataName->text());

    QList<QBarSet*> barsets = static_cast<QBarSeries*>(chartView->chart()->series().at(0))->barSets();
    for (auto it = barsets.begin(); it != barsets.end(); it++)
        if ((*it)->label() == chartDataName)
            (*it)->setLabel(newChartDataName->text());

    return QPair<QString,QString>(newChartDataName->text(),"");

}
