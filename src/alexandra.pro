 ###################################################################
#                                                                   #
#  Alexandra Video Library                                          #
#  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>        #
#                                                                   #
 ###################################################################

TARGET = alexandra
TEMPLATE = app

QT = core gui widgets concurrent
CONFIG += qt warn_on thread debug
QMAKE_CXXFLAGS += -std=c++11
DEFINES += _UNICODE UNICODE

LIBS += -lstdc++ -lpthread
linux:LIBS += -lmediainfo
win32:LIBS += $$PWD\..\lib\MediaInfo.dll
win32:INCLUDEPATH += $$PWD\..\lib\

DESTDIR = ./
MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp/obj
RCC_DIR = ./tmp/rcc
UI_DIR = ./tmp/ui

HEADERS += aboutwindow.h \
           addfilmwindow.h \
           alexandrasettings.h \
           editfilmwindow.h \
           film.h \
           filminfowindow.h \
           filmslist.h \
           mainwindow.h \           
           searchwindow.h \
           settingswindow.h \
           statusbar.h \
           toolbar.h \
           version.h

SOURCES += aboutwindow.cpp \
           addfilmwindow.cpp \
           alexandra.cpp \
           alexandrasettings.cpp \
           editfilmwindow.cpp \
           film.cpp \
           filminfowindow.cpp \
           filmslist.cpp \
           mainwindow.cpp \
           searchwindow.cpp \
           settingswindow.cpp \
           statusbar.cpp \
           toolbar.cpp

FORMS += aboutwindow.ui \
         addfilmwindow.ui \
         filminfowindow.ui \
         mainwindow.ui \
         searchwindow.ui \
         settingswindow.ui

TRANSLATIONS += ../lang/alexandra-cs.ts \
                ../lang/alexandra-ru.ts \
                ../lang/alexandra-uk.ts

RESOURCES = icons.qrc \
            images.qrc \
            lang.qrc
