#include "mainwidget.h"

#include "Control/controller.h"
#include "charttab.h"
#include "Model/BarChart/barchart.h"
#include "Model/LineChart/linechart.h"
#include "Model/PieChart/piechart.h"
#include "BarChart/barcharttab.h"
#include "LineChart//linecharttab.h"
#include "PieChart/piecharttab.h"

/* CONSTRUCTOR & DESTRUCTOR */
MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    desktopHeight = screenGeometry.height();
    desktopWidth = screenGeometry.width();
    resize (desktopWidth * 0.9, desktopHeight * 0.75);
    move (desktopWidth * 0.05, desktopHeight * 0.125);
    setMinimumHeight(desktopHeight * 0.375);
    setMinimumWidth(desktopWidth * 0.4);

    QVBoxLayout* macroLayout = new QVBoxLayout();

    macroLayout->addLayout(buttons());

    tab = new QTabWidget();
    tab->setMovable(false);
    macroLayout->addWidget(tab);

    _tabsOpened = false;
    QLabel* introTab = new QLabel("Creare un nuovo file o aprirne uno");
    introTab->setAlignment(Qt::AlignCenter);
    introTab->setStyleSheet("font: 20pt;");

    tab->addTab(introTab, "Aprire un file");

    setAcceptDrops(true);
    setLayout(macroLayout);
}

MainWidget::~MainWidget() {}

/* SETUP HEADER -> BUTTONS */
QHBoxLayout* MainWidget::buttons() {
    openFileBtn = new QPushButton();
    newFileBtn = new QPushButton();
    saveFileBtn = new QPushButton();
    saveAsBtn = new QPushButton();
    exportPDFBtn = new QPushButton();

    openFileBtn->setToolTip("Apri");
    newFileBtn->setToolTip("Nuovo grafico");
    saveFileBtn->setToolTip("Salva");
    saveAsBtn->setToolTip("Salva con nome");
    exportPDFBtn->setToolTip("Esporta come PDF");

    openFileBtn->setIcon(QIcon(QPixmap(":/Static/openfile.png")));
    newFileBtn->setIcon(QIcon(QPixmap(":/Static/newfile.png")));
    saveFileBtn->setIcon(QIcon(QPixmap(":/Static/save.png")));
    saveAsBtn->setIcon(QIcon(QPixmap(":/Static/saveas.png")));
    exportPDFBtn->setIcon(QIcon(QPixmap(":/Static/savePDF.png")));

    openFileBtn->setIconSize(QSize(height() * 0.06, height() * 0.06));
    newFileBtn->setIconSize(QSize(height() * 0.06, height() * 0.06));
    saveFileBtn->setIconSize(QSize(height() * 0.06, height() * 0.06));
    saveAsBtn->setIconSize(QSize(height() * 0.06, height() * 0.06));
    exportPDFBtn->setIconSize(QSize(height() * 0.06, height() * 0.06));

    openFileBtn->setStyleSheet("border:0px;");
    newFileBtn->setStyleSheet("border:0px;");
    saveFileBtn->setStyleSheet("border:0px;");
    saveAsBtn->setStyleSheet("border:0px;");
    exportPDFBtn->setStyleSheet("border:0px;");

    openFileBtn->installEventFilter(this);
    newFileBtn->installEventFilter(this);
    saveFileBtn->installEventFilter(this);
    saveAsBtn->installEventFilter(this);
    exportPDFBtn->installEventFilter(this);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(newFileBtn);
    btnLayout->addWidget(openFileBtn);
    btnLayout->addWidget(saveFileBtn);
    btnLayout->addWidget(saveAsBtn);
    btnLayout->addWidget(exportPDFBtn);
    btnLayout->addSpacerItem(new QSpacerItem(width() * 0.8, 0));

    return btnLayout;
}

bool MainWidget::eventFilter (QObject* object, QEvent* event) {
    QPushButton* obj = static_cast<QPushButton*>(object);
    if (event->type() == QEvent::Enter) {
        if (obj == saveFileBtn)
            saveFileBtn->setIcon(QIcon(QPixmap(":/Static/hovered_save.png")));
        else if (obj == saveAsBtn)
            saveAsBtn->setIcon(QIcon(QPixmap(":/Static/hovered_saveas.png")));
        else if (obj == newFileBtn)
            newFileBtn->setIcon(QIcon(QPixmap(":/Static/hovered_newfile.png")));
        else if (obj == openFileBtn)
            openFileBtn->setIcon(QIcon(QPixmap(":/Static/hovered_openfile.png")));
        else if (obj == exportPDFBtn)
            exportPDFBtn->setIcon(QIcon(QPixmap(":/Static/hovered_savePDF.png")));
    } else if (event->type() == QEvent::Leave) {
        if (obj == saveFileBtn)
            saveFileBtn->setIcon(QIcon(QPixmap(":/Static/save.png")));
        else if (obj == saveAsBtn)
            saveAsBtn->setIcon(QIcon(QPixmap(":/Static/saveas.png")));
        else if (obj == newFileBtn)
            newFileBtn->setIcon(QIcon(QPixmap(":/Static/newfile.png")));
        else if (obj == openFileBtn)
            openFileBtn->setIcon(QIcon(QPixmap(":/Static/openfile.png")));
        else if (obj == exportPDFBtn)
            exportPDFBtn->setIcon(QIcon(QPixmap(":/Static/savePDF.png")));
    }
    return false;
}

/* LINK TO CONTROLLER */
void MainWidget::setController (Controller* _controller) {
    controller = _controller;
    connectButtons();
}

/* CONNECTION SIGNAL-SLOTS */
void MainWidget::connectButtons() {
    connect (this, SIGNAL(fileDropped(QString)), controller, SLOT(openFile(QString)));
    connect (this, SIGNAL(closing(QCloseEvent*)), controller, SLOT(checkAllFilesSaved(QCloseEvent*)));

    connect (tab, SIGNAL(tabCloseRequested(int)), controller, SLOT(closeTab(int)));

    connect (openFileBtn, SIGNAL(clicked()), controller, SLOT(openFile()));
    connect (saveFileBtn, SIGNAL(clicked()), controller, SLOT(saveFile()));
    connect (saveAsBtn, SIGNAL(clicked()), controller, SLOT(saveAs()));
    connect (newFileBtn, SIGNAL(clicked()), controller, SLOT(newFile()));
    connect (exportPDFBtn, SIGNAL(clicked()), controller, SLOT(exportPDF()));
}

// ADDS A CHARTTAB
void MainWidget::addNewTab(Chart* x) {
    ChartTab* chartTab;
    if (dynamic_cast<BarChart*>(x))
        chartTab = new BarChartTab(x);
    else if (dynamic_cast<LineChart*>(x))
        chartTab = new LineChartTab(x);
    else if (dynamic_cast<PieChart*>(x))
        chartTab = new PieChartTab(x);
    else
        return;

    /* TROVO IL TITOLO DEL TAB */
    unsigned int first_character_position = x->getPath().toStdString().find_last_of("/") + 1;
    QString title = QString::fromStdString(x->getPath().toStdString().substr(first_character_position, x->getPath().length()));

    connect (chartTab->getNewChartDataBtn(), SIGNAL(clicked()), controller, SLOT(newChartData()));
    connect (chartTab->getDelChartDataBtn(), SIGNAL(clicked()), controller, SLOT(delChartData()));

    if (dynamic_cast<BarChartTab*>(chartTab)) {
        connect (static_cast<BarChartTab*>(chartTab)->getNewCatBtn(), SIGNAL(clicked()), controller, SLOT(newCategory()));
        connect (static_cast<BarChartTab*>(chartTab)->getDelCatBtn(), SIGNAL(clicked()), controller, SLOT(delCategory()));
    }

    tab->addTab(chartTab, title);
    tab->setCurrentWidget(chartTab);



    tab->resize(width(), height());         // PER FAR PARTIRE IL RESIZE DI CHARTTAB

    if (!_tabsOpened) {
        tab->removeTab(0);
        tab->setTabsClosable(true);
        _tabsOpened = true;
    }
    connectChartDataButtons();
    if (dynamic_cast<BarChartTab*>(chartTab) || dynamic_cast<LineChartTab*>(chartTab))
        connectSubButtons();
}

/* DRAG AND DROP */

void MainWidget::dragEnterEvent(QDragEnterEvent* e) {
    e->acceptProposedAction();
}

void MainWidget::dropEvent(QDropEvent* e) {
    if (e->mimeData()->hasUrls())
        for (const auto &i: e->mimeData()->urls())
            emit fileDropped(QString::fromStdString(i.toString().toStdString().substr(7, i.toString().size())));
}

/* DIALOGS */
void MainWidget::okDialog (const QString& message) {
    QMessageBox::information(this, "", message);
}

void MainWidget::errorDialog(const QString& message) {
    QMessageBox::critical(this, "Errore", message);
}

// OPENS THE DIALOG THAT GET THE FILEPATH
QString MainWidget::openFileDialog (const QString& folder) {
    return QFileDialog::getOpenFileName(this, "Seleziona il file", folder, "JSON files (*.json)");
}

// ASK IF YOU WANT TO SAVE OR NOT BEFORE CLOSING
SaveOptions MainWidget::fileNotSavedDialog() {
    QMessageBox::StandardButton reply = QMessageBox::question (this, "Salvare?", "File non salvato, salvare?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    switch (reply) {
        case QMessageBox::Yes:
            return SaveOptions::Save;
        case QMessageBox::No:
            return SaveOptions::DontSave;
        case QMessageBox::Cancel:
            return SaveOptions::Cancel;
        default:
            return SaveOptions::Cancel;
    }
}

// GET WHERE TO SAVE A CHART WITH A FILE THAT DOESNT EXISTS
QString MainWidget::savePathDialog (const QString& folder) {
    return QFileDialog::getSaveFileName(this, "Salva in", folder, "JSON files(*.json)");
}

bool MainWidget::yesOrNoDialog (const QString& msg) {
    QMessageBox::StandardButton reply = QMessageBox::question (this, "", msg, QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
        return true;
    return false;
}

QString MainWidget::chooseTypeDialog() {
    bool ok = false;
    QStringList items;
    items << "Grafico a barre" << "Grafico con linee" << "Grafico a torta";
    QString x = QInputDialog::getItem(this, "Tipo", "Seleziona il tipo: ", items, 0, false, &ok, Qt::Dialog);
    if (ok)
        return x;
    return "";
}

void MainWidget::closeTab (const int& i) {
    if (i < tab->count() && i >= 0)
        tab->removeTab(i);
    if (tab->count() == 0) {
        tab->setTabsClosable(false);
        QLabel* introTab = new QLabel("Creare un nuovo file o aprirne uno");
        introTab->setAlignment(Qt::AlignCenter);
        introTab->setStyleSheet("font: 20pt;");
        tab->addTab(introTab, "Aprire un file");
        _tabsOpened = false;
    }
}

bool MainWidget::tabsOpened() const {
    return _tabsOpened;
}

int MainWidget::currentTabIndex() const {
    return tab->currentIndex();
}

/* CAMBIA LA SCRITTA SUL TITOLO DELLO SPECIFICO TAB IN CASO VENGA CAMBIATO */
void MainWidget::updatePath (const QString& newPath) {
    unsigned int first_character_position = newPath.toStdString().find_last_of("/") + 1;
    QString title = QString::fromStdString(newPath.toStdString().substr(first_character_position, newPath.length()));
    tab->setTabText(tab->currentIndex(), title);
}

void MainWidget::closeEvent (QCloseEvent* e) {
    emit closing(e);
}

/* RIMBALZO CHIAMATE */
QStringList MainWidget::addChartData() {
    QPair<QStringList,bool> tmp = static_cast<ChartTab*>(tab->currentWidget())->addChartDataDialog();
    if (!tmp.first.isEmpty()) {
        if (!tmp.second) {
            errorDialog("Nome già inserito");
            return QStringList();
        }
        static_cast<ChartTab*>(tab->currentWidget())->addChartData(tmp.first);
        QPushButton* btn = static_cast<ChartTab*>(tab->currentWidget())->getChartDataOptionButtons().last();
        connect (btn, SIGNAL(clicked()), controller, SLOT(chartDataOptions()));
        if (dynamic_cast<BarChartTab*>(tab->currentWidget())) {
            QList<QPushButton*> buttons = static_cast<ChartTab*>(tab->currentWidget())->getSubOptionButtons().last();
            for (auto i: buttons)
                connect (i, SIGNAL(clicked()), controller, SLOT(subOptions()));
        }
    }
    return tmp.first;
}

void MainWidget::connectChartDataButtons() {
    QList<QPushButton*> buttons = (static_cast<ChartTab*>(tab->currentWidget())->getChartDataOptionButtons());
    for (auto i: buttons)
        connect (i, SIGNAL(clicked()), controller, SLOT(chartDataOptions()));
}

void MainWidget::connectSubButtons() {
    QList<QList<QPushButton*>> buttons = (static_cast<ChartTab*>(tab->currentWidget())->getSubOptionButtons());
    for (auto& i: buttons)
        for (auto j: i)
            connect (j, SIGNAL(clicked()), controller, SLOT(subOptions()));
}

QString MainWidget::delChartDataDialog() {
    return static_cast<ChartTab*>(tab->currentWidget())->delChartDataDialog();
}

bool MainWidget::delChartData (const QString& chartDataName) {
    return static_cast<ChartTab*>(tab->currentWidget())->delChartData(chartDataName);
}

QPair<QString,QString> MainWidget::modChartData(const QString& chartDataName) {
    return static_cast<ChartTab*>(tab->currentWidget())->modChartData(chartDataName);
}

void MainWidget::exportPDF (const QString& folder) {
    static_cast<ChartTab*>(tab->currentWidget())->exportPDF(folder);
}

QPair<QString,QString> MainWidget::showChartDataOptionsMenu (QPushButton* sender) {
    return static_cast<ChartTab*>(tab->currentWidget())->showChartDataOptions(sender);
}

QPair<double,double> MainWidget::addNewPoint (const QString& chartDataName) {
    QList<double> ret = static_cast<LineChartTab*>(tab->currentWidget())->addNewPoint(chartDataName);
    if (ret.isEmpty())
        return QPair<double,double>();
    QPushButton* btn = static_cast<LineChartTab*>(tab->currentWidget())->getSubOptionButtons().at(ret.at(2)).last();
    connect (btn, SIGNAL(clicked()), controller, SLOT(subOptions()));
    return QPair<double,double>(ret.at(0),ret.at(1));
}

QPair<QString,int> MainWidget::delPoint () {
    return static_cast<LineChartTab*>(tab->currentWidget())->deletePoint();
}

QList<QVariant> MainWidget::modSubChartData() {
    return static_cast<ChartTab*>(tab->currentWidget())->modSubChartData();
}

QString MainWidget::newCategory() {
    QString ret = static_cast<BarChartTab*>(tab->currentWidget())->addCategory();
    QList<QList<QPushButton*>> buttons = static_cast<BarChartTab*>(tab->currentWidget())->getSubOptionButtons();
    for (auto& i: buttons)
        connect (i.at(i.size()-1), SIGNAL(clicked()), controller, SLOT(subOptions()));
    return ret;
}

QString MainWidget::delCategory() {
    return static_cast<BarChartTab*>(tab->currentWidget())->delCategory();
}

void MainWidget::showSubOptionsMenu(QPushButton* sender) {
    QPair<QAction*,QAction*> actions = static_cast<ChartTab*>(tab->currentWidget())->showSubOptions(sender);
    if (actions.first)
        connect (actions.first, SIGNAL(triggered()), controller, SLOT(modSubChartData()));
    if (actions.second)
        connect (actions.second, SIGNAL(triggered()), controller, SLOT(delPoint()));
    static_cast<ChartTab*>(tab->currentWidget())->execMenu();
}
