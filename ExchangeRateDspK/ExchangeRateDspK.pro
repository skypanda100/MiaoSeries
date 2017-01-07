#-------------------------------------------------
#
# Project created by QtCreator 2016-12-24T12:32:34
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExchangeRateDspK
TEMPLATE = app

DEFINES += CHARTDIR_HIDE_OBSOLETE _CRT_SECURE_NO_WARNINGS

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    qchartviewer.cpp \
    inputwidget.cpp \
    style.cpp \
    dataresultwidget.cpp \
    graphresultwidget.cpp \
    result.cpp \
    db.cpp \
    simulate.cpp

HEADERS  += mainwindow.h \
    qchartviewer.h \
    inputwidget.h \
    style.h \
    dataresultwidget.h \
    graphresultwidget.h \
    result.h \
    db.h \
    simulate.h

INCLUDEPATH += $$PWD/include
win32:LIBS += $$PWD/lib/chartdir60.lib

RESOURCES += \
    res.qrc
