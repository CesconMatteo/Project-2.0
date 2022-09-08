QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Control/controller.cpp \
    Model/BarChart/barchart.cpp \
    Model/BarChart/barset.cpp \
    Model/LineChart/line.cpp \
    Model/LineChart/linechart.cpp \
    Model/PieChart/piechart.cpp \
    Model/PieChart/pieslice.cpp \
    Model/chart.cpp \
    Model/chartdata.cpp \
    Model/model.cpp \
    View/BarChart/barcharttab.cpp \
    View/LineChart/linecharttab.cpp \
    View/PieChart/piecharttab.cpp \
    View/charttab.cpp \
    View/mainwidget.cpp \
    main.cpp

HEADERS += \
    Control/controller.h \
    Model/BarChart/barchart.h \
    Model/BarChart/barset.h \
    Model/LineChart/line.h \
    Model/LineChart/linechart.h \
    Model/PieChart/piechart.h \
    Model/PieChart/pieslice.h \
    Model/chart.h \
    Model/chartdata.h \
    Model/model.h \
    View/BarChart/barcharttab.h \
    View/LineChart/linecharttab.h \
    View/PieChart/piecharttab.h \
    View/charttab.h \
    View/mainwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
