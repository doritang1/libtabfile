QT += core
QT -= gui

TARGET = libtabfile
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    libtabfile.cpp

HEADERS += \
    libtabfile.h

