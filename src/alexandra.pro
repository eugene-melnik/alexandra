 ###################################################################
#                                                                   #
#  Alexandra Video Library                                          #
#  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>        #
#                                                                   #
 ###################################################################

TARGET = alexandra
TEMPLATE = app

#CONFIG += release
CONFIG += debug

# Main configuration
QT = core gui widgets
CONFIG += qt warn_on thread

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -Ofast 

LIBS += -lstdc++ -lpthread

win32:INCLUDEPATH += $$PWD\..\lib\
win32:RC_FILE = $$PWD\..\windows\win-meta.rc

# Options
DEFINES += _UNICODE UNICODE
DEFINES += MEDIAINFO_SUPPORT
#DEFINES += PORTABLE_VERSION

# Source files
HEADERS += effects/effectdropshadow.h \
           aboutwindow.h \
           abstractfilmsview.h \
           addfilmwindow.h \
           alexandrasettings.h \
           commandlineparser.h \
           editfilmwindow.h \
           filesextensions.h \
           film.h \
           filmscannerwindow.h \
           filmscannerworker.h \
           filmscanneraddworker.h \
           filmslist.h \
           filmsviewcontextmenu.h \
           filmsviewgrid.h \
           filmsviewgridmodel.h \
           filmsviewlist.h \
           mainwindow.h \
           movedfilmswindow.h \
           playlist.h \
           playlistwidget.h \
           searchedit.h \
           searchwindow.h \
           settingswindow.h \
           splashscreen.h \
           statisticswindow.h \
           statisticsworker.h \
           statusbar.h \
           timecounter.h \
           toolbar.h \
           version.h

SOURCES += effects/effectdropshadow.cpp \
           aboutwindow.cpp \
           addfilmwindow.cpp \
           alexandrasettings.cpp \
           commandlineparser.cpp \
           editfilmwindow.cpp \
           filesextensions.cpp \
           film.cpp \
           filmscannerwindow.cpp \
           filmscannerworker.cpp \
           filmscanneraddworker.cpp \
           filmslist.cpp \
           filmsviewcontextmenu.cpp \
           filmsviewgrid.cpp \
           filmsviewgridmodel.cpp \
           filmsviewlist.cpp \
           main.cpp \
           mainwindow.cpp \
           movedfilmswindow.cpp \
           playlist.cpp \
           playlistwidget.cpp \
           searchedit.cpp \
           searchwindow.cpp \
           settingswindow.cpp \
           splashscreen.cpp \
           statisticswindow.cpp \
           statisticsworker.cpp \
           statusbar.cpp \
           timecounter.cpp \
           toolbar.cpp

FORMS += aboutwindow.ui \
         addfilmwindow.ui \
         filmscannerwindow.ui \
         mainwindow.ui \
         movedfilmswindow.ui \
         searchwindow.ui \
         settingswindow.ui \
         splashscreen.ui \
         statisticswindow.ui

TRANSLATIONS += ../lang/alexandra-cs.ts \
                ../lang/alexandra-fr.ts \
                ../lang/alexandra-ru_RU.ts \
                ../lang/alexandra-uk_UA.ts

RESOURCES = icons.qrc \
            images.qrc \
            lang.qrc

# MediaInfo support
contains( DEFINES, MEDIAINFO_SUPPORT ) {
    message( "MediaInfo support enabled." )

    HEADERS += filminfowindow.h mediainfo.h
    SOURCES += filminfowindow.cpp mediainfo.cpp
    FORMS += filminfowindow.ui

    linux:LIBS += -lmediainfo
    win32:LIBS += $$PWD\..\lib\MediaInfo.dll
}

# Portable version
contains( DEFINES, PORTABLE_VERSION ) {
    message( "Portable version." )
}

# Temporary files
DESTDIR = ./
MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp/obj
RCC_DIR = ./tmp/rcc
UI_DIR = ./tmp/ui

# Stylesheets
RESOURCES += ../styles/alexandraflat-coast/alexandraflat-coast.qrc \
             ../styles/alexandraflat-rainforest/alexandraflat-rainforest.qrc \
             ../styles/qdarkstylesheet/qdarkstylesheet.qrc

# Installation setup
target.path = /usr/bin/

desktop_file.files = ../linux/alexandra.desktop
desktop_file.path = /usr/share/applications/

desktop_file_addfilms.files = ../linux/alexandra_addfilms.desktop
desktop_file_addfilms.path = /usr/share/applications/

icon_png.files = ../linux/alexandra.png
icon_png.path = /usr/share/icons/

icon_svg.files = ../linux/alexandra.svg
icon_svg.path = /usr/share/icons/hicolor/scalable/apps/

INSTALLS = target \
           desktop_file \
           desktop_file_addfilms \
           icon_png \
           icon_svg
