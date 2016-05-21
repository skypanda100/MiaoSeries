#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T16:13:38
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExchangeRateDsp
TEMPLATE = app

INCLUDEPATH += $$PWD/include
RC_FILE += \
    ea.rc

DEFINES += CHARTDIR_HIDE_OBSOLETE _CRT_SECURE_NO_WARNINGS

win32:LIBS += $$PWD/lib/chartdir60.lib

SOURCES += main.cpp\
    qchartviewer.cpp \
    ea_db.cpp \
    mainwidget.cpp \
    searchwidget.cpp \
    ea_result.cpp \
    detailwidget.cpp \
    ratecolordialog.cpp \
    graphwidget.cpp \
    resultwidget.cpp \
    window.cpp \
    iconbutton.cpp

HEADERS  += \
    qchartviewer.h \
    ea_db.h \
    mainwidget.h \
    searchwidget.h \
    ea_cons.h \
    ea_result.h \
    detailwidget.h \
    ratecolordialog.h \
    graphwidget.h \
    resultwidget.h \
    window.h \
    iconbutton.h

RESOURCES += data.qrc
