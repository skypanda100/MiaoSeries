QT += core sql
QT -= gui

TARGET = ExchangeRateValidate
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ea_db.cpp

HEADERS += \
    ea_db.h

