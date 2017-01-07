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
    ea_db.cpp \
    ea_result.cpp \
    style.cpp \
    dataresultwidget.cpp \
    graphresultwidget.cpp

HEADERS  += mainwindow.h \
    qchartviewer.h \
    inputwidget.h \
    ea_db.h \
    ea_result.h \
    style.h \
    dataresultwidget.h \
    graphresultwidget.h

INCLUDEPATH += $$PWD/include
win32:LIBS += $$PWD/lib/chartdir60.lib

RESOURCES += \
    res.qrc
