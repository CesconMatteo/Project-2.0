#include "piecharttab.h"

#include "Model/PieChart/piechart.h"
#include "Model/PieChart/pieslice.h"

PieChartTab::PieChartTab(Chart* chart, QWidget* parent) : ChartTab(parent) {
    QVBoxLayout* macroLayout = new QVBoxLayout();
    macroLayout->addLayout(header());

    if (chart->empty()) {
        zeroDataTab("Inserire una fetta");
        macroLayout->addWidget(zeroDataLabel);
        voidChart = true;
    } else {
        QHBoxLayout* horizontalLayout = new QHBoxLayout();
        setupScroll(chart);
        horizontalLayout->addWidget(scroll);
        dxLayout(chart);
        horizontalLayout->addWidget(chartView);
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
        sliceName->setMaximumWidth(100);
        sliceName->setFont(font);
        sliceName->setAlignment(Qt::AlignCenter);
        firstLayout->addWidget(sliceName);
        chartDataNames.push_back(sliceName);

        /* BUTTON */
        QPushButton* btn = new QPushButton("Opzioni");
        firstLayout->addWidget(btn);
        chartDataOptionButtons.push_back(btn);

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
        QList<QWidget*> tmpSC = QList<QWidget*>();
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

    dataLayout->setSizeConstraint(QLayout::SetFixedSize);

    /* SET UP SCROLL */
    QWidget* scrollWidget = new QWidget();
    scrollWidget->setLayout(dataLayout);
    scroll = new QScrollArea();
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll->setWidget(scrollWidget);
    scroll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    scroll->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
}

void PieChartTab::dxLayout (Chart* chart) {

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
        static_cast<QLineEdit*>(secondColoumn.at(i).at(0))->setText(QString::fromStdString(value.str()));
    }

    for (int i=0; i < chartDataNames.size(); i++) {
        QPieSlice* slice = static_cast<QPieSeries*>(chartView->chart()->series().at(0))->slices().at(i);
        slice->setLabel(chartDataNames.at(i)->text().append(" ").append(QString::number(slice->percentage() * 100, 'f', 1)).append("%"));
    }

}

QStringList PieChartTab::addChartDataDialog() {
    QDialog dialogWindow(this);
    dialogWindow.setWindowTitle("Nuova fetta");
    QFormLayout* layout = new QFormLayout();
    QLineEdit* name = new QLineEdit();
    QLineEdit* value = new QLineEdit();
    value->setValidator(new QRegularExpressionValidator(QRegularExpression("[+-]?([0-9]*[.])?[0-9]+")));
    layout->addRow("Nome", name);
    layout->addRow("Valore", value);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialogWindow);
    connect(&buttonBox, SIGNAL(accepted()), &dialogWindow, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialogWindow, SLOT(reject()));
    layout->addRow(&buttonBox);
    dialogWindow.setLayout(layout);
    name->setFocus();
    QStringList results = QStringList();
    if (dialogWindow.exec() == QDialog::Accepted) {
        results.push_back(name->text());
        results.push_back(value->text());
    } else
        return QStringList();

    for (auto i: chartDataNames)
        if (i->text() == results.at(0))
            return QStringList("errore");

    return results;
}

void PieChartTab::addChartData(const QStringList& info) {

    if (!voidChart) {
        QPieSlice* slice = new QPieSlice(info.at(0) + " ", info.at(1).toDouble());
        connect (slice, SIGNAL(hovered(bool)), this, SLOT(explodeSlice(bool)));
        static_cast<QPieSeries*>(chartView->chart()->series().at(0))->append(slice);

        QVBoxLayout* newLayout = new QVBoxLayout();
        QHBoxLayout* firstLayout = new QHBoxLayout();
        QHBoxLayout* secondLayout = new QHBoxLayout();

        QLabel* label = new QLabel(info.at(0));
        QFont font = label->font();
        font.setBold(true);
        font.setPointSize(20);
        label->setFont(font);
        label->setMaximumWidth(100);
        label->setAlignment(Qt::AlignCenter);
        firstLayout->addWidget(label);
        chartDataNames.push_back(label);

        QPushButton* setOptionBtn = new QPushButton("Opzioni");         // ADD BUTTON
        firstLayout->addWidget(setOptionBtn);
        chartDataOptionButtons.push_back(setOptionBtn);

        std::stringstream value;
        if (fmod(info.at(1).toDouble(), 1.0) == 0)
            value.precision(0);
        else
            value.precision(2);
        value << std::fixed;
        value << info.at(1).toDouble();
        QLineEdit* valueEdit = new QLineEdit(QString::fromStdString(value.str()));
        valueEdit->setAlignment(Qt::AlignCenter);
        secondLayout->addWidget(valueEdit);
        QList<QWidget*> tmpFC = QList<QWidget*>();
        tmpFC.push_back(valueEdit);
        firstColoumn.push_back(tmpFC);

        QLineEdit* percEdit = new QLineEdit();
        percEdit->setAlignment(Qt::AlignCenter);
        QList<QWidget*> tmp2 = QList<QWidget*>();
        tmp2.push_back(percEdit);
        secondColoumn.push_back(tmp2);
        secondLayout->addWidget(percEdit);

        newLayout->addLayout(firstLayout);
        newLayout->addLayout(secondLayout);

        QFrame* separator = createSeparator();
        newLayout->addWidget(separator);
        chartDataSeparators.push_back(separator);

        static_cast<QVBoxLayout*>(scroll->widget()->layout())->addLayout(newLayout);

        updatePercentage();

    } else {

        PieChart* x = new PieChart();
        PieSlice* y = new PieSlice(info.at(0), info.at(1).toDouble());
        x->push_back(y);

        QHBoxLayout* horizontalLayout = new QHBoxLayout();
        setupScroll(x);
        horizontalLayout->addWidget(scroll);
        dxLayout(x);
        horizontalLayout->addWidget(chartView);
        delete zeroDataLabel;
        zeroDataLabel = nullptr;
        static_cast<QVBoxLayout*>(layout())->addLayout(horizontalLayout);
        delete x;
        voidChart = false;
        updatePercentage();
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

        delete firstColoumn.at(k).at(0);
        delete secondColoumn.at(k).at(0);
        delete chartDataOptionButtons.at(k);
        delete chartDataNames.at(k);
        delete chartDataSeparators.at(k);

        firstColoumn.removeAt(k);
        secondColoumn.removeAt(k);
        chartDataOptionButtons.removeAt(k);
        chartDataNames.removeAt(k);
        chartDataSeparators.removeAt(k);

        updatePercentage();

        /* CONTROLLO SE E' VUOTO */
        if (firstColoumn.size() == 0) {
            voidChart = true;
            delete scroll;
            delete chartView;

            zeroDataTab("Inserire una fetta");
            static_cast<QVBoxLayout*>(layout())->addWidget(zeroDataLabel);
        }

        return true;
    }
    return false;
}

QPair<QString, QString> PieChartTab::modChartData(const QString& chartDataName) {

    int k = -1;
    for (int i=0; i < chartDataNames.size(); i++)
        if (chartDataNames.at(i)->text() == chartDataName)
            k=i;

    QDialog dialogWindow(this);
    dialogWindow.setWindowTitle("Modifica nome set");

    QFormLayout* layout = new QFormLayout();
    QLineEdit* newChartDataName = new QLineEdit(chartDataName);
    layout->addRow("Nome fetta", newChartDataName);
    QLineEdit* newValue = new QLineEdit(static_cast<QLineEdit*>(firstColoumn.at(k).at(0))->text());
    newValue->setValidator(new QRegularExpressionValidator(QRegularExpression("[+-]?([0-9]*[.])?[0-9]+")));
    layout->addRow("Valore", newValue);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialogWindow);
    connect(&buttonBox, SIGNAL(accepted()), &dialogWindow, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialogWindow, SLOT(reject()));
    layout->addRow(&buttonBox);
    dialogWindow.setLayout(layout);
    newChartDataName->setFocus();

    if (!(dialogWindow.exec() == QDialog::Accepted))
        return QPair<QString,QString>();
    if (newChartDataName->text().isEmpty())
        return QPair<QString,QString>();

    chartDataNames.at(k)->setText(newChartDataName->text());


    static_cast<QLineEdit*>(firstColoumn.at(k).at(0))->setText(newValue->text());

    for (auto i: static_cast<QPieSeries*>(chartView->chart()->series().at(0))->slices()){
        QString str = QString::fromStdString(i->label().toStdString().substr(0, i->label().toStdString().find(' ')));
        if (str == chartDataName) {
            static_cast<QPieSeries*>(chartView->chart()->series().at(0))->remove(i);
        }
    }

    QPieSlice* newSlice = new QPieSlice(newChartDataName->text(), newValue->text().toDouble());
    connect (newSlice, SIGNAL(hovered(bool)), this, SLOT(explodeSlice(bool)));
    static_cast<QPieSeries*>(chartView->chart()->series().at(0))->insert(k,newSlice);
    updatePercentage();

    return QPair<QString,QString>(newChartDataName->text(),newValue->text());
}

