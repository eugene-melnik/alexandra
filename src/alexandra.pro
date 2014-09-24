TARGET = alexandra
TEMPLATE = app

QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt warn_on debug
QMAKE_CXXFLAGS += -std=c++11

HEADERS += alexandra.h \
           mainwindow.h \
           version.h

SOURCES += alexandra.cpp \
           mainwindow.cpp

FORMS += mainwindow.ui

