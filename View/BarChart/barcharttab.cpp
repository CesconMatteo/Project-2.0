#include "barcharttab.h"

#include "Model/BarChart/barchart.h"
#include "Model/BarChart/barset.h"

BarChartTab::BarChartTab(Chart* chart, QWidget* parent) : ChartTab(parent) {
    QVBoxLayout* macroLayout = new QVBoxLayout();
    macroLayout->addLayout(header());

    if (chart->empty() && static_cast<BarChart*>(chart)->categories().isEmpty()) {
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
    newCatBtn = new QPushButton("Nuova categoria");
    delCatBtn = new QPushButton("Elimina categoria");
    newChartDataBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    delChartDataBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    newCatBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    delCatBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    layout->addWidget(newChartDataBtn);
    layout->addWidget(delChartDataBtn);
    layout->addWidget(newCatBtn);
    layout->addWidget(delCatBtn);
    return layout;
}

void BarChartTab::setupScroll (Chart* chart) {

    QVBoxLayout* dataLayout = new QVBoxLayout();

    firstColoumn = QList<QList<QWidget*>>();
    secondColoumn = QList<QList<QWidget*>>();

    for (auto it = chart->begin(); it != chart->end(); it++) {

        /* SET NAME LAYOUT */
        QHBoxLayout* setLayout = new QHBoxLayout();
        QLabel* setName = new QLabel((*it)->getName());                 // ADD LABEL
        QFont font = setName->font();
        font.setBold(true);
        font.setPointSize(20);
        setName->setFont(font);
        setName->setFixedWidth(150);
        setName->setAlignment(Qt::AlignCenter);
        setLayout->addWidget(setName);
        chartDataNames.push_back(setName);

        QPushButton* setOptionBtn = new QPushButton("Opzioni");         // ADD BUTTON
        setLayout->addWidget(setOptionBtn);
        dataLayout->addLayout(setLayout);
        chartDataOptionButtons.push_back(setOptionBtn);

        QVBoxLayout* externalLayout = new QVBoxLayout();
        QList<QWidget*> tmpFC = QList<QWidget*>();
        QList<QWidget*> tmpSC = QList<QWidget*>();
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

        chartDataLayouts.push_back(externalLayout);

        QFrame* separator = createSeparator();
        chartDataSeparators.push_back(separator);

        dataLayout->addLayout(externalLayout);
        dataLayout->addWidget(separator);
    }

    dataLayout->setSizeConstraint(QLayout::SetFixedSize);

    /* SET UP SCROLL */
    QWidget* scrollWidget = new QWidget();
    scrollWidget->setLayout(dataLayout);
    scrollWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    scroll = new QScrollArea();
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    scroll->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
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

    QValueAxis* axisY = new QValueAxis();
    graphicChart->addAxis(axisY, Qt::AlignLeft);
    axisY->setTickCount(10);
    series->attachAxis(axisY);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    graphicChart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

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
            if (static_cast<QLineEdit*>(j)->text().toDouble() > max)
                max = static_cast<QLineEdit*>(j)->text().toDouble();
            if (static_cast<QLineEdit*>(j)->text().toDouble() < min)
                min = static_cast<QLineEdit*>(j)->text().toDouble();
        }
    }
    static_cast<QValueAxis*>(chartView->chart()->axes(Qt::Vertical).at(0))->setRange(min,max);
}

QPair<QStringList,bool> BarChartTab::addChartDataDialog() {
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
            return QPair<QStringList,bool>(QStringList(),true);
        results.push_back(setName->text());
        for (auto i: edits)
            if (!i->text().isEmpty())
                results.push_back(i->text());
            else
                results.push_back("0");
    } else
        return QPair<QStringList,bool>(QStringList(),true);

    for (auto i: chartDataNames)
        if (i->text() == results.at(0))
            return QPair<QStringList,bool>(results,false);

    return QPair<QStringList,bool>(results,true);;
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
        QFont font = setName->font();
        font.setBold(true);
        font.setPointSize(20);
        setName->setFont(font);
        setName->setFixedWidth(150);
        setName->setAlignment(Qt::AlignCenter);
        setLayout->addWidget(setName);
        chartDataNames.push_back(setName);

        QPushButton* setOptionBtn = new QPushButton("Opzioni");         // ADD BUTTON
        setLayout->addWidget(setOptionBtn);
        chartDataOptionButtons.push_back(setOptionBtn);

        QVBoxLayout* externalLayout = new QVBoxLayout();
        externalLayout->addLayout(setLayout);
        QList<QWidget*> tmpFC = QList<QWidget*>();
        QList<QWidget*> tmpSC = QList<QWidget*>();
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

        delete chartView->chart()->axes(Qt::Orientation::Horizontal).at(0);
        delete chartView->chart()->axes(Qt::Orientation::Vertical).at(0);

        chartView->chart()->removeAllSeries();
        chartView->chart()->addSeries(newBarSeries);

        QValueAxis* axisY = new QValueAxis();
        chartView->chart()->addAxis(axisY, Qt::AlignLeft);
        axisY->setTickCount(10);
        newBarSeries->attachAxis(axisY);

        QBarCategoryAxis* axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chartView->chart()->addAxis(axisX, Qt::AlignBottom);
        newBarSeries->attachAxis(axisX);

        newBarSeries->attachAxis(axisX);
        newBarSeries->attachAxis(axisY);

        static_cast<QVBoxLayout*>(scroll->widget()->layout())->addLayout(externalLayout);
        chartDataLayouts.push_back(externalLayout);
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

QString BarChartTab::addCategory() {

    if (voidChart) {
        QMessageBox::critical(this, "Errore", "Inserire prima un set");
        return "";
    }

    QString ret = "";

    QDialog dialogWindow(this);
    dialogWindow.setWindowTitle("Nuova categoria");
    QFormLayout* layout = new QFormLayout();
    QLineEdit* newCategory = new QLineEdit();
    layout->addRow("Nome categoria", newCategory);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialogWindow);
    connect(&buttonBox, SIGNAL(accepted()), &dialogWindow, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialogWindow, SLOT(reject()));
    layout->addRow(&buttonBox);
    dialogWindow.setLayout(layout);
    newCategory->setFocus();
    if (dialogWindow.exec() == QDialog::Accepted)
        if (!newCategory->text().isEmpty())
            ret = newCategory->text();
        else
            return "";
    else
        return "";

    categories.push_back(ret);

    for (int i=0; i < chartDataLayouts.size(); i++) {
        QHBoxLayout* newLine = new QHBoxLayout();

        QLabel* label = new QLabel(ret);
        label->setAlignment(Qt::AlignCenter);
        newLine->addWidget(label);
        firstColoumn[i].push_back(label);

        QLineEdit* edit = new QLineEdit("0");
        edit->setAlignment(Qt::AlignCenter);
        newLine->addWidget(edit);
        secondColoumn[i].push_back(edit);

        QPushButton* btn = new QPushButton("···");
        newLine->addWidget(btn);
        subOptionButtons[i].push_back(btn);

        chartDataLayouts.at(i)->addLayout(newLine);
    }

    static_cast<QBarCategoryAxis*>(chartView->chart()->axes(Qt::Orientation::Horizontal).at(0))->append(ret);

    return ret;
}

QString BarChartTab::delCategory() {

    if (voidChart || categories.isEmpty())
        return "";

    bool ok = false;
    QStringList items;
    for (auto& i: categories)
        items << i;
    QString x = QInputDialog::getItem(this, "Eliminazione", "Seleziona quale categoria eliminare: ", items, 0, false, &ok, Qt::Dialog);
    if (!ok)
        return "";

    int i;
    for (i=0; i < categories.size(); i++)
        if (categories.at(i) == x)
            break;

    categories.removeAt(i);
    for (int j=0; j < secondColoumn.size(); j++) {
        delete firstColoumn.at(j).at(i);
        delete secondColoumn.at(j).at(i);
        delete subOptionButtons.at(j).at(i);
        firstColoumn[j].removeAt(i);
        secondColoumn[j].removeAt(i);
        subOptionButtons[j].removeAt(i);
    }

    delete static_cast<QBarSeries*>(chartView->chart()->series().at(0))->attachedAxes().at(1);
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chartView->chart()->addAxis(axisX, Qt::AlignBottom);
    chartView->chart()->series().at(0)->attachAxis(axisX);
    for (auto j: static_cast<QBarSeries*>(chartView->chart()->series().at(0))->barSets())
        j->remove(i);

    return x;
}

QPair<QAction*,QAction*> BarChartTab::showSubOptions (QPushButton* sender) {
    menu = new QMenu("Opzioni", this);
    QAction* modifyAction = new QAction("Modifica", this);
    menu->addAction(modifyAction);
    QPoint a = sender->mapToGlobal(QPoint(sender->width()/2, sender->height()/2));
    menu->move(a.x(), a.y());

    int i = -1;
    int j = -1;
    for (int _i = 0; _i < subOptionButtons.size(); _i++)
        for (int _j = 0; _j < subOptionButtons.at(_i).size(); _j++)
            if (subOptionButtons.at(_i).at(_j) == sender) {
                i = _i;
                j = _j;
            }
    buttonIndexes = QPair<int,int>(i,j);

    return QPair<QAction*,QAction*>(modifyAction, nullptr);
}

QList<QVariant> BarChartTab::modSubChartData() {

    QDialog dialogWindow(this);
    dialogWindow.setWindowTitle("Modifica valore");

    QFormLayout* layout = new QFormLayout();

    QLineEdit* edit = new QLineEdit();
    layout->addRow("Valore: ", edit);
    edit->setValidator(new QRegularExpressionValidator(QRegularExpression("[+-]?([0-9]*[.])?[0-9]+")));

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialogWindow);
    connect(&buttonBox, SIGNAL(accepted()), &dialogWindow, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialogWindow, SLOT(reject()));
    layout->addRow(&buttonBox);

    dialogWindow.setLayout(layout);
    edit->setFocus();

    QList<QVariant> res = QList<QVariant>();
    if (dialogWindow.exec() == QDialog::Accepted) {
        if (edit->text().isEmpty())
            res.push_back(0);
        else
            res.push_back(edit->text().toDouble());
    } else
        return QList<QVariant>();
    static_cast<QLineEdit*>(secondColoumn.at(buttonIndexes.first).at(buttonIndexes.second))->setText(QString::number(res.at(0).toDouble()));

    static_cast<QBarSeries*>(chartView->chart()->series().at(0))->barSets().at(buttonIndexes.first)->remove(buttonIndexes.second,1);
    static_cast<QBarSeries*>(chartView->chart()->series().at(0))->barSets().at(buttonIndexes.first)->insert(buttonIndexes.second,res.at(0).toDouble());

    res.push_back(chartDataNames.at(buttonIndexes.first)->text());
    res.push_back(buttonIndexes.second);

    return res;
}

QPushButton* BarChartTab::getNewCatBtn() const {
    return newCatBtn;
}

QPushButton* BarChartTab::getDelCatBtn() const {
    return delCatBtn;
}
