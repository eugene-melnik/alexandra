 ###################################################################
#                                                                   #
#  Alexandra Video Library                                          #
#  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>        #
#                                                                   #
 ###################################################################

TARGET = alexandra
TEMPLATE = app

QT = core gui widgets concurrent

CONFIG += qt warn_on debug
QMAKE_CXXFLAGS += -std=c++11 #-flto
DEFINES += _UNICODE UNICODE

LIBS += -lz -lpthread -lstdc++

linux:LIBS += -lmediainfo

win32:LIBS += $$PWD\..\lib\MediaInfo.dll
win32:INCLUDEPATH += $$PWD\..\lib\

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

TRANSLATIONS += lang/alexandra-cs.ts \
                lang/alexandra-ru.ts \
                lang/alexandra-uk.ts

RESOURCES = lang/lang.qrc \
            icons/icons.qrc \
            images/images.qrc
