#-------------------------------------------------
#
# Project created by QtCreator 2016-11-14T16:03:57
#
#-------------------------------------------------

QT       += core gui
QT       += xml widgets
QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testCAD
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
     iconsCatalog.cpp \
    combiner/combinerWindow.cpp \
    dataTable/dataTable.cpp \
    dataTable/tableModel.cpp \
    tree/treeAnalyzer.cpp \
    tree/treeEditor.cpp \
    tree/treeOpener.cpp \
    tree/treeSaver.cpp \
    splashWindow.cpp \
    sequencer/bezierController.cpp \
    sequencer/connector.cpp \
    sequencer/diagram.cpp \
    sequencer/diagramView.cpp \
    sequencer/floatingTextBox.cpp \
    sequencer/sequencerWindow.cpp \
    sequencer/stateShape.cpp \
    sequencer/textBox.cpp \
    sequencer/trigonometry.cpp \
    sequencer/fsmRouter.cpp \
    crossChecker/crossCheckerWindow.cpp \
    statsWindow/statsWindow.cpp \
    collector/collectorWindow.cpp

HEADERS  += mainwindow.h \
     iconsCatalog.h \
    combiner/combinerWindow.h \
    dataTable/dataTable.h \
    dataTable/tableModel.h \
    tree/treeAnalyzer.h \
    tree/treeEditor.h \
    tree/treeOpener.h \
    tree/treeSaver.h \
    splashWindow.h \
    sequencer/bezierController.h \
    sequencer/connector.h \
    sequencer/diagram.h \
    sequencer/diagramView.h \
    sequencer/floatingTextBox.h \
    sequencer/sequencerWindow.h \
    sequencer/stateShape.h \
    sequencer/textBox.h \
    sequencer/trigonometry.h \
    testCAD.h \
    sequencer/fsmRouter.h \
    crossChecker/crossCheckerWindow.h \
    statsWindow/statsWindow.h \
    collector/collectorWindow.h

FORMS    +=

TRANSLATIONS    = testCAD_en.ts \
                  testCAD_cn.ts \
                  testCAD_es.ts \
                  testCAD_ca.ts \
                  testCAD_ru.ts
RESOURCES += \
    graphics.qrc

RC_FILE = appIcon.rc
