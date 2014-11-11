TARGET = alexandra
TEMPLATE = app

QT = core gui widgets

CONFIG += qt warn_on release
QMAKE_CXXFLAGS += -std=c++11
LIBS += -lmediainfo #-lz -lpthread -lzen -lpthread -lstdc++

HEADERS += aboutwindow.h \
           addfilmwindow.h \
           alexandra.h \
           editfilmwindow.h \
           film.h \
           filmslist.h \
           mainwindow.h \
           settingswindow.h \
           toolbar.h \
           version.h

SOURCES += aboutwindow.cpp \
           addfilmwindow.cpp \
           alexandra.cpp \
           editfilmwindow.cpp \
           film.cpp \
           filmslist.cpp \
           mainwindow.cpp \
           settingswindow.cpp \
           toolbar.cpp

FORMS += aboutwindow.ui \
         addfilmwindow.ui \
         mainwindow.ui \
         settingswindow.ui

TRANSLATIONS += lang/alexandra-ru.ts \
                lang/alexandra-uk.ts

RESOURCES = lang/lang.qrc \
            icons/icons.qrc \
            images/images.qrc

