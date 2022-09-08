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
    exportPDFBtn->setText("Esporta come PDF");

    openFileBtn->setIconSize(QSize(height() * 0.04, height() * 0.04));
    newFileBtn->setIconSize(QSize(height() * 0.04, height() * 0.04));
    saveFileBtn->setIconSize(QSize(height() * 0.04, height() * 0.04));
    saveAsBtn->setIconSize(QSize(height() * 0.04, height() * 0.04));
    exportPDFBtn->setIconSize(QSize(height() * 0.04, height() * 0.04));

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
    } else if (event->type() == QEvent::Leave) {
        if (obj == saveFileBtn)
            saveFileBtn->setIcon(QIcon(QPixmap(":/Static/save.png")));
        else if (obj == saveAsBtn)
            saveAsBtn->setIcon(QIcon(QPixmap(":/Static/saveas.png")));
        else if (obj == newFileBtn)
            newFileBtn->setIcon(QIcon(QPixmap(":/Static/newfile.png")));
        else if (obj == openFileBtn)
            openFileBtn->setIcon(QIcon(QPixmap(":/Static/openfile.png")));
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
    connect (this, SIGNAL(closing()), controller, SLOT(checkAllFilesSaved()));

    connect (tab, SIGNAL(tabCloseRequested(int)), controller, SLOT(closeTab(int)));

    connect (openFileBtn, SIGNAL(clicked()), controller, SLOT(openFile()));
    connect (saveFileBtn, SIGNAL(clicked()), controller, SLOT(saveFile()));
    connect (saveAsBtn, SIGNAL(clicked()), controller, SLOT(saveAs()));
    connect (newFileBtn, SIGNAL(clicked()), controller, SLOT(newFile()));
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

    tab->addTab(chartTab, title);
    tab->setCurrentWidget(chartTab);



    tab->resize(width(), height());         // PER FAR PARTIRE IL RESIZE DI CHARTTAB

    if (!_tabsOpened) {
        tab->removeTab(0);
        tab->setTabsClosable(true);
        _tabsOpened = true;
    }
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
    QMessageBox::StandardButton reply = QMessageBox::question (this, "Salvare?", "File non salvato, chiudere lo stesso?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
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
    return QFileDialog::getSaveFileName(this, "Salva come", folder, "JSON files(*.json)");
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

void MainWidget::closeEvent (QCloseEvent* ) {
    emit closing();
}

/* RIMBALZO CHIAMATE */
QString MainWidget::delChartDataDialog() {
    return static_cast<ChartTab*>(tab->currentWidget())->delChartDataDialog();
}

bool MainWidget::delChartData (const QString& chartDataName) {
    return static_cast<ChartTab*>(tab->currentWidget())->delChartData(chartDataName);
}