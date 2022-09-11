QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

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

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
