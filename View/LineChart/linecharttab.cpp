#include "linecharttab.h"

#include "Model/LineChart/linechart.h"
#include "Model/LineChart/line.h"

LineChartTab::LineChartTab(Chart* chart, QWidget* parent) : ChartTab(parent) {
    QVBoxLayout* macroLayout = new QVBoxLayout();
    macroLayout->addLayout(header());

    if (chart->empty()) {
        zeroDataTab("Inserire una linea");
        macroLayout->addWidget(zeroDataLabel);
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
        label->setAlignment(Qt::AlignCenter);
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
            xEdit->setFixedWidth(width() * 0.15);
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
            yEdit->setFixedWidth(width() * 0.15);
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

    dataLayout->setSizeConstraint(QLayout::SetFixedSize);

    /* SET UP SCROLL */
    QWidget* scrollWidget = new QWidget();
    scrollWidget->setLayout(dataLayout);
    scroll = new QScrollArea();
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
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

QStringList LineChartTab::addChartDataDialog() {
    QDialog dialogWindow(this);
    dialogWindow.setWindowTitle("Nuova linea");
    QFormLayout* layout = new QFormLayout();
    QLineEdit* title = new QLineEdit();
    layout->addRow("Nome linea", title);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialogWindow);
    connect(&buttonBox, SIGNAL(accepted()), &dialogWindow, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialogWindow, SLOT(reject()));
    layout->addRow(&buttonBox);
    dialogWindow.setLayout(layout);
    title->setFocus();
    QStringList tmp = QStringList();
    if (dialogWindow.exec() == QDialog::Accepted)
        if (!title->text().isEmpty())
            tmp.push_back(title->text());
    return tmp;
}

/* DA CONTROLLARE PER EVENTUALI LAYOUT CHE RESTANO IN MEMORIA */
void LineChartTab::addChartData(const QStringList& info) {

    if (!voidChart) {
        QLineSeries* newLine = new QLineSeries();
        newLine->setName(info.at(0));
        chartView->chart()->addSeries(newLine);

        QVBoxLayout* newLayout = new QVBoxLayout();
        QHBoxLayout* internalLayout = new QHBoxLayout();

        QLabel* label = new QLabel(info.at(0));                         // ADD LABEL
        QFont font = label->font();
        font.setBold(true);
        font.setPointSize(20);
        label->setFont(font);
        internalLayout->addWidget(label);
        chartDataNames.push_back(label);

        QPushButton* setOptionBtn = new QPushButton("Opzioni");         // ADD BUTTON
        internalLayout->addWidget(setOptionBtn);
        chartDataOptionButtons.push_back(setOptionBtn);

        newLayout->addLayout(internalLayout);

        QFrame* separator = createSeparator();
        newLayout->addWidget(separator);
        chartDataSeparators.push_back(separator);

        firstColoumn.push_back(QList<QWidget*>());
        secondColoumn.push_back(QList<QLineEdit*>());
        subOptionButtons.push_back(QList<QPushButton*>());

        static_cast<QVBoxLayout*>(scroll->widget()->layout())->addLayout(newLayout);

    } else {
        LineChart* x = new LineChart();
        Line* y = new Line(info.at(0));
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
    }
}

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

            zeroDataTab("Inserire una linea");
            static_cast<QVBoxLayout*>(layout())->addWidget(zeroDataLabel);
        }

        return true;
    }
    return false;
}

QPair<QString,QString> LineChartTab::showChartDataOptions (QPushButton* sender) {
    QMenu menu("Opzioni", this);
    QAction* modifyAction = new QAction("Modifica", this);
    QAction* deleteAction = new QAction("Elimina", this);
    QAction* newPointAction = new QAction("Nuovo punto", this);
    menu.addAction(modifyAction);
    menu.addAction(newPointAction);
    menu.addAction(deleteAction);
    QPoint a = sender->mapToGlobal(QPoint(sender->width()/2, sender->height()/2));
    menu.move(a.x(), a.y());
    QAction* x = menu.exec();
    if (x)
        for (int i=0; i < chartDataOptionButtons.size(); i++)
            if (chartDataOptionButtons.at(i) == sender)
                return QPair<QString,QString>(x->text(),chartDataNames.at(i)->text());
    return QPair<QString,QString>();
}

QPair<QString, QString> LineChartTab::modChartData(const QString& chartDataName) {
    QDialog dialogWindow(this);
    dialogWindow.setWindowTitle("Modifica nome set");

    QFormLayout* layout = new QFormLayout();
    QLineEdit* newChartDataName = new QLineEdit(chartDataName);
    layout->addRow("Nome linea", newChartDataName);

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

    for (auto i: chartView->chart()->series())
        if (i->name() == chartDataName)
            i->setName(newChartDataName->text());

    return QPair<QString,QString>(newChartDataName->text(),"");
}
