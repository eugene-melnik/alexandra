 ###################################################################
#                                                                   #
#  Alexandra Video Library                                          #
#  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>        #
#                                                                   #
 ###################################################################

CONFIG += release
#CONFIG += debug

TARGET = alexandra
TEMPLATE = app

QT = core gui widgets concurrent
CONFIG += qt warn_on thread
DEFINES += _UNICODE UNICODE
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -Ofast -flto

LIBS += -lstdc++ -lpthread
linux:LIBS += -lmediainfo

win32:RC_FILE = $$PWD\win-meta.rc
win32:LIBS += $$PWD\..\lib\MediaInfo.dll
win32:INCLUDEPATH += $$PWD\..\lib\

DESTDIR = ./
MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp/obj
RCC_DIR = ./tmp/rcc
UI_DIR = ./tmp/ui

HEADERS += effects/effectdropshadow.h \
           aboutwindow.h \
           abstractfilmsview.h \
           addfilmwindow.h \
           alexandrasettings.h \
           editfilmwindow.h \
           film.h \
           filminfowindow.h \
           filmscannerwindow.h \
           filmslist.h \
           filmsviewcontextmenu.h \
           filmsviewgrid.h \
           filmsviewgridmodel.h \
           filmsviewlist.h \
           mainwindow.h \
           mediainfo.h \
           movedfilmswindow.h \
           searchwindow.h \
           settingswindow.h \
           splashscreen.h \
           statusbar.h \
           timecounter.h \
           toolbar.h \
           version.h

SOURCES += effects/effectdropshadow.cpp \
           aboutwindow.cpp \
           addfilmwindow.cpp \
           alexandrasettings.cpp \
           editfilmwindow.cpp \
           film.cpp \
           filminfowindow.cpp \
           filmscannerwindow.cpp \
           filmslist.cpp \
           filmsviewcontextmenu.cpp \
           filmsviewgrid.cpp \
           filmsviewgridmodel.cpp \
           filmsviewlist.cpp \
           main.cpp \
           mainwindow.cpp \
           mediainfo.cpp \
           movedfilmswindow.cpp \
           searchwindow.cpp \
           settingswindow.cpp \
           splashscreen.cpp \
           statusbar.cpp \
           timecounter.cpp \
           toolbar.cpp

FORMS += aboutwindow.ui \
         addfilmwindow.ui \
         filminfowindow.ui \
         filmscannerwindow.ui \
         mainwindow.ui \
         movedfilmswindow.ui \
         searchwindow.ui \
         settingswindow.ui \
         splashscreen.ui

TRANSLATIONS += ../lang/alexandra-cs.ts \
                ../lang/alexandra-fr.ts \
                ../lang/alexandra-ru_RU.ts \
                ../lang/alexandra-uk_UA.ts

RESOURCES = icons.qrc \
            images.qrc \
            lang.qrc

# Stylesheets
RESOURCES += ../styles/alexandraflat-rainforest/alexandraflat-rainforest.qrc \
             ../styles/qdarkstylesheet/qdarkstylesheet.qrc

# Install
target.path = /usr/bin/

desktop_file.files = ../linux/alexandra.desktop
desktop_file.path = /usr/share/applications/

icon_png.files = ../linux/alexandra.png
icon_png.path = /usr/share/icons/

icon_svg.files = ../linux/alexandra.svg
icon_svg.path = /usr/share/icons/hicolor/scalable/apps/

INSTALLS = target desktop_file icon_png icon_svg
