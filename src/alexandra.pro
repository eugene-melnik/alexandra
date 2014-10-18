TARGET = alexandra
TEMPLATE = app

QT = core gui widgets

CONFIG += qt warn_on debug
QMAKE_CXXFLAGS += -std=c++11

HEADERS += aboutwindow.h \
           addfilmwindow.h \
           alexandra.h \
           film.h \
           filmslist.h \
           mainwindow.h \
           version.h

SOURCES += aboutwindow.cpp \
           addfilmwindow.cpp \
           alexandra.cpp \
           film.cpp \
           filmslist.cpp \
           mainwindow.cpp

FORMS += aboutwindow.ui \
         addfilmwindow.ui \
         mainwindow.ui

TRANSLATIONS += lang/alexandra-ru.ts \
                lang/alexandra-uk.ts

RESOURCES = lang/lang.qrc \
            icons/icons.qrc \
            images/images.qrc

