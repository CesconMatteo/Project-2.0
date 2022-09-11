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

    firstColoumn = QList<QList<QWidget*>>();
    secondColoumn = QList<QList<QWidget*>>();

    for (auto it = chart->begin(); it != chart->end(); it++) {

        /* LINE NAME LAYOUT */
        QHBoxLayout* lineNameLayout = new QHBoxLayout();
        QLabel* label = new QLabel((*it)->getName());
        QFont font = label->font();
        font.setBold(true);
        font.setPointSize(20);
        label->setFont(font);
        label->setFixedWidth(150);
        label->setAlignment(Qt::AlignCenter);
        lineNameLayout->addWidget(label);
        chartDataNames.push_back(label);

        QPushButton* setOptionBtn = new QPushButton("Opzioni");         // ADD BUTTON
        lineNameLayout->addWidget(setOptionBtn);
        dataLayout->addLayout(lineNameLayout);
        chartDataOptionButtons.push_back(setOptionBtn);

        QVBoxLayout* externalLayout = new QVBoxLayout();
        QList<QWidget*> tmpFC = QList<QWidget*>();
        QList<QWidget*> tmpSC = QList<QWidget*>();
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
            internalLayout->addWidget(btn);
            tmpBtn.push_back(btn);

            externalLayout->addLayout(internalLayout);
        }

        firstColoumn.push_back(tmpFC);
        secondColoumn.push_back(tmpSC);
        subOptionButtons.push_back(tmpBtn);

        QFrame* separator = createSeparator();
        chartDataSeparators.push_back(separator);

        chartDataLayouts.push_back(externalLayout);
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
    scroll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    scroll->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);    
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
    static_cast<QValueAxis*>(graphicChart->axes(Qt::Orientation::Horizontal).at(0))->setTickCount(10);
    static_cast<QValueAxis*>(graphicChart->axes(Qt::Orientation::Vertical).at(0))->setTickCount(10);
    graphicChart->axes(Qt::Orientation::Horizontal).at(0)->setTitleText("X");
    graphicChart->axes(Qt::Orientation::Vertical).at(0)->setTitleText("Y");
    chartView = new QChartView(graphicChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

QPair<QStringList,bool> LineChartTab::addChartDataDialog() {
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
    if (dialogWindow.exec() == QDialog::Accepted) {
        if (!title->text().isEmpty())
            tmp.push_back(title->text());
    } else
        return QPair<QStringList,bool>(QStringList(),true);

    for (auto i: chartDataNames)
        if (i->text() == tmp.at(0)) {
            return QPair<QStringList,bool>(tmp,false);
        }

    return QPair<QStringList,bool>(tmp,true);
}

/* DA CONTROLLARE PER EVENTUALI LAYOUT CHE RESTANO IN MEMORIA */
void LineChartTab::addChartData(const QStringList& info) {

    if (!voidChart) {
        QLineSeries* newLine = new QLineSeries();
        newLine->setName(info.at(0));
        chartView->chart()->addSeries(newLine);

        QVBoxLayout* newLayout = new QVBoxLayout();
        QHBoxLayout* internalLayout = new QHBoxLayout();
        QVBoxLayout* externalLayout = new QVBoxLayout();

        QLabel* label = new QLabel(info.at(0));                         // ADD LABEL
        QFont font = label->font();
        font.setBold(true);
        font.setPointSize(20);
        label->setFont(font);
        label->setFixedWidth(150);
        label->setAlignment(Qt::AlignCenter);
        internalLayout->addWidget(label);
        chartDataNames.push_back(label);

        chartDataLayouts.push_back(externalLayout);
        externalLayout->addLayout(internalLayout);

        QPushButton* setOptionBtn = new QPushButton("Opzioni");         // ADD BUTTON
        internalLayout->addWidget(setOptionBtn);
        chartDataOptionButtons.push_back(setOptionBtn);

        newLayout->addLayout(externalLayout);

        QFrame* separator = createSeparator();
        newLayout->addWidget(separator);
        chartDataSeparators.push_back(separator);

        firstColoumn.push_back(QList<QWidget*>());
        secondColoumn.push_back(QList<QWidget*>());
        subOptionButtons.push_back(QList<QPushButton*>());

        static_cast<QVBoxLayout*>(scroll->widget()->layout())->addLayout(newLayout);
        resizeAxis();

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
        delete chartDataLayouts.at(k);

        firstColoumn.removeAt(k);
        secondColoumn.removeAt(k);
        subOptionButtons.removeAt(k);
        chartDataOptionButtons.removeAt(k);
        chartDataNames.removeAt(k);
        chartDataSeparators.removeAt(k);
        chartDataLayouts.removeAt(k);

        /* CONTROLLO SE E' VUOTO */
        if (firstColoumn.size() == 0) {
            voidChart = true;
            delete scroll;
            delete chartView;

            zeroDataTab("Inserire una linea");
            static_cast<QVBoxLayout*>(layout())->addWidget(zeroDataLabel);
        }
        resizeAxis();

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

    resizeAxis();
    return QPair<QString,QString>(newChartDataName->text(),"");
}

QList<double> LineChartTab::addNewPoint (const QString& chartDataName) {

    QDialog dialogWindow(this);
    dialogWindow.setWindowTitle("Nuovo punto");

    QFormLayout* layout = new QFormLayout();

    QLineEdit* x = new QLineEdit();
    layout->addRow("X:", x);
    x->setValidator(new QRegularExpressionValidator(QRegularExpression("[+-]?([0-9]*[.])?[0-9]+")));
    QLineEdit* y = new QLineEdit();
    layout->addRow("Y:", y);
    y->setValidator(new QRegularExpressionValidator(QRegularExpression("[+-]?([0-9]*[.])?[0-9]+")));

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialogWindow);
    connect(&buttonBox, SIGNAL(accepted()), &dialogWindow, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialogWindow, SLOT(reject()));
    layout->addRow(&buttonBox);

    dialogWindow.setLayout(layout);
    x->setFocus();

    QList<double> res = QList<double>();
    if (dialogWindow.exec() == QDialog::Accepted) {
        if (x->text().isEmpty())
            res.push_back(0);
        else
            res.push_back(x->text().toDouble());
        if (y->text().isEmpty())
            res.push_back(0);
        else
            res.push_back(y->text().toDouble());
    } else
        return res;

    int index;
    for (index=0; index < chartDataNames.size(); index++)
        if (chartDataNames.at(index)->text() == chartDataName)
            break;

    res.push_back(index);

    QHBoxLayout* internalLayout = new QHBoxLayout();
    /* LINEEDIT X */
    stringstream xStream;
    if (fmod(res.at(0), 1.0) == 0)
        xStream.precision(0);
    else
        xStream.precision(2);
    xStream << std::fixed;
    xStream << res.at(0);
    QLineEdit* xEdit = new QLineEdit(QString::fromStdString(xStream.str()));
    xEdit->setAlignment(Qt::AlignCenter);
    internalLayout->addWidget(xEdit);
    firstColoumn[index].push_back(xEdit);

    /* LINEEDIT Y */
    stringstream yStream;
    if (fmod(res.at(1), 1.0) == 0)
        yStream.precision(0);
    else
        yStream.precision(2);
    yStream << std::fixed;
    yStream << res.at(1);
    QLineEdit* yEdit = new QLineEdit(QString::fromStdString(yStream.str()));
    yEdit->setAlignment(Qt::AlignCenter);
    internalLayout->addWidget(yEdit);
    secondColoumn[index].push_back(yEdit);

    /* BUTTON */
    QPushButton* btn = new QPushButton("···");
    internalLayout->addWidget(btn);
    subOptionButtons[index].push_back(btn);

    chartDataLayouts.at(index)->addLayout(internalLayout);

    static_cast<QLineSeries*>(chartView->chart()->series().at(index))->append(res.at(0),res.at(1));

    resizeAxis();
    return res;
}

QPair<QAction*,QAction*> LineChartTab::showSubOptions(QPushButton* sender) {
    menu = new QMenu("Opzioni", this);
    QAction* modifyAction = new QAction("Modifica", this);
    QAction* deleteAction = new QAction("Elimina", this);
    menu->addAction(modifyAction);
    menu->addAction(deleteAction);
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

    return QPair<QAction*,QAction*>(modifyAction,deleteAction);
}

QPair<QString,int> LineChartTab::deletePoint() {
    delete firstColoumn.at(buttonIndexes.first).at(buttonIndexes.second);
    delete secondColoumn.at(buttonIndexes.first).at(buttonIndexes.second);
    delete subOptionButtons.at(buttonIndexes.first).at(buttonIndexes.second);

    firstColoumn[buttonIndexes.first].removeAt(buttonIndexes.second);
    secondColoumn[buttonIndexes.first].removeAt(buttonIndexes.second);
    subOptionButtons[buttonIndexes.first].removeAt(buttonIndexes.second);

    static_cast<QLineSeries*>(chartView->chart()->series().at(buttonIndexes.first))->remove(buttonIndexes.second);

    resizeAxis();
    return QPair<QString,int>(chartDataNames.at(buttonIndexes.first)->text(),buttonIndexes.second);
}

QList<QVariant> LineChartTab::modSubChartData() {

    QDialog dialogWindow(this);
    dialogWindow.setWindowTitle("Modifica punto");

    QFormLayout* layout = new QFormLayout();

    QLineEdit* x = new QLineEdit();
    layout->addRow("X:", x);
    x->setValidator(new QRegularExpressionValidator(QRegularExpression("[+-]?([0-9]*[.])?[0-9]+")));
    QLineEdit* y = new QLineEdit();
    layout->addRow("Y:", y);
    y->setValidator(new QRegularExpressionValidator(QRegularExpression("[+-]?([0-9]*[.])?[0-9]+")));

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialogWindow);
    connect(&buttonBox, SIGNAL(accepted()), &dialogWindow, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialogWindow, SLOT(reject()));
    layout->addRow(&buttonBox);

    dialogWindow.setLayout(layout);
    x->setFocus();

    QList<QVariant> res = QList<QVariant>();
    if (dialogWindow.exec() == QDialog::Accepted) {
        if (x->text().isEmpty())
            res.push_back(0);
        else
            res.push_back(x->text().toDouble());
        if (y->text().isEmpty())
            res.push_back(0);
        else
            res.push_back(y->text().toDouble());
    } else
        return QList<QVariant>();

    static_cast<QLineEdit*>(firstColoumn.at(buttonIndexes.first).at(buttonIndexes.second))->setText(QString::number(res.at(0).toDouble()));
    static_cast<QLineEdit*>(secondColoumn.at(buttonIndexes.first).at(buttonIndexes.second))->setText(QString::number(res.at(1).toDouble()));

    static_cast<QLineSeries*>(chartView->chart()->series().at(buttonIndexes.first))->remove(buttonIndexes.second);
    static_cast<QLineSeries*>(chartView->chart()->series().at(buttonIndexes.first))->insert(buttonIndexes.second, QPoint(res.at(0).toDouble(),res.at(1).toDouble()));

    res.push_back(chartDataNames.at(buttonIndexes.first)->text());
    res.push_back(buttonIndexes.second);

    resizeAxis();
    return res;
}

void LineChartTab::resizeAxis() {
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::min();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::min();
    for (int i=0; i < firstColoumn.size(); i++) {
        for (int j=0; j < firstColoumn.at(i).size(); j++) {
            if (static_cast<QLineEdit*>(firstColoumn.at(i).at(j))->text().toDouble() > maxX)
                maxX = static_cast<QLineEdit*>(firstColoumn.at(i).at(j))->text().toDouble();
            if (static_cast<QLineEdit*>(firstColoumn.at(i).at(j))->text().toDouble() < minX)
                minX = static_cast<QLineEdit*>(firstColoumn.at(i).at(j))->text().toDouble();
            if (static_cast<QLineEdit*>(secondColoumn.at(i).at(j))->text().toDouble() > maxY)
                maxY = static_cast<QLineEdit*>(secondColoumn.at(i).at(j))->text().toDouble();
            if (static_cast<QLineEdit*>(secondColoumn.at(i).at(j))->text().toDouble() < minY)
                minY = static_cast<QLineEdit*>(secondColoumn.at(i).at(j))->text().toDouble();
        }
    }
    chartView->chart()->axes(Qt::Orientation::Horizontal).at(0)->setRange(minX, maxX);
    chartView->chart()->axes(Qt::Orientation::Vertical).at(0)->setRange(minY, maxY);
}
