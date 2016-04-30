QT += core sql
QT -= gui

TARGET = ExchangeRateMa
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

RC_FILE += \
    ea.rc

SOURCES += main.cpp \
    ea_db.cpp

HEADERS += \
    ea_db.h

