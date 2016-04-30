#-------------------------------------------------
#
# Project created by QtCreator 2016-03-05T14:16:01
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExchangeRateService
TEMPLATE = app

RC_FILE += \
    ea.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    ea_net.cpp \
    ea_db.cpp

HEADERS  += mainwindow.h \
    ea_net.h \
    ea_db.h

RESOURCES += service.qrc
