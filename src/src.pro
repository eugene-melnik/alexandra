 ###################################################################
#                                                                   #
#  Alexandra Video Library                                          #
#  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>        #
#                                                                   #
 ###################################################################

    # Main configuration #

TARGET = alexandra
TEMPLATE = app


#CONFIG += release
#CONFIG += debug console

QT = core gui widgets network
CONFIG += qt warn_on thread

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -Ofast

LIBS += -lstdc++ -lpthread

win32:RC_FILE = $$PWD\..\windows\win-meta.rc
win32:INCLUDEPATH += $$PWD\..\lib\


    # Options #

DEFINES += _UNICODE UNICODE
DEFINES += MEDIAINFO_SUPPORT
#DEFINES += PORTABLE_VERSION


    # Source files #

HEADERS += effects/effectdropshadow.h \
           filmslist/old/film0x10.h \
           filmslist/old/filmslistoldloader.h \
           filmslist/filmitem.h \
           filmslist/filmslistmodel.h \
           filmslist/filmslistproxymodel.h \
           filmsview/grid/filmsviewgrid.h \
           filmsview/grid/filmsviewgridproxymodel.h \
           filmsview/info/abstractfilminfoview.h \
           filmsview/info/filminfoview.h \
           filmsview/info/filmposterview.h \
           filmsview/info/filmtechinfoview.h \
           filmsview/list/filmsviewlist.h \
           filmsview/abstractfilmsview.h \
           filmsview/filmsviewcontextmenu.h \
           network/networkrequest.h \
           network/updatechecker.h \
#           parsers/abstractparser.h \
#           parsers/parsermanager.h \
#           parsers/imdb/imdbparser.h \
#           parsers/kinopoisk/kinopoiskparser.h \
#           parsers/kinoteatr/kinoteatrparser.h \
#           parsers/omdb/omdbparser.h \
           search/searchedit.h \
#           search/searchwindow.h \
#           scanner/filmscannerwindow.h \
#           scanner/filmscannerworker.h \
#           scanner/filmscanneraddworker.h \
#           scanner/movedfilmswindow.h \
           tools/filesextensions.h \
           tools/regexptools.h \
           tools/timecounter.h \
           aboutwindow.h \
#           addfilmwindow.h \
           alexandrasettings.h \
           commandlineparser.h \
#           editfilmwindow.h \
           mainwindow.h \
           playlist.h \
           playlistwidget.h \
           settingswindow.h \
           splashscreen.h \
#           statisticswindow.h \
#           statisticsworker.h \
           statusbar.h \
           toolbar.h \
           version.h

SOURCES += effects/effectdropshadow.cpp \
           filmslist/old/film0x10.cpp \
           filmslist/old/filmslistoldloader.cpp \
           filmslist/filmitem.cpp \
           filmslist/filmslistmodel.cpp \
           filmsview/grid/filmsviewgrid.cpp \
           filmsview/grid/filmsviewgridproxymodel.cpp \
           filmsview/info/filminfoview.cpp \
           filmsview/info/filmposterview.cpp \
           filmsview/info/filmtechinfoview.cpp \
           filmsview/list/filmsviewlist.cpp \
           filmsview/filmsviewcontextmenu.cpp \
           network/networkrequest.cpp \
           network/updatechecker.cpp \
#           parsers/parsermanager.cpp \
#           parsers/imdb/imdbparser.cpp \
#           parsers/kinopoisk/kinopoiskparser.cpp \
#           parsers/kinoteatr/kinoteatrparser.cpp \
#           parsers/omdb/omdbparser.cpp \
           search/searchedit.cpp \
#           search/searchwindow.cpp \
#           scanner/filmscannerwindow.cpp \
#           scanner/filmscannerworker.cpp \
#           scanner/filmscanneraddworker.cpp \
#           scanner/movedfilmswindow.cpp \
           tools/filesextensions.cpp \
           tools/regexptools.cpp \
           tools/timecounter.cpp \
           aboutwindow.cpp \
#           addfilmwindow.cpp \
           alexandrasettings.cpp \
           commandlineparser.cpp \
#           editfilmwindow.cpp \
           main.cpp \
           mainwindow.cpp \
           playlist.cpp \
           playlistwidget.cpp \
           settingswindow.cpp \
           splashscreen.cpp \
#           statisticswindow.cpp \
#           statisticsworker.cpp \
           statusbar.cpp \
           toolbar.cpp

FORMS += filmsview/info/filminfoview.ui \
#         search/searchwindow.ui \
#         scanner/filmscannerwindow.ui \
#         scanner/movedfilmswindow.ui \
         aboutwindow.ui \
#         addfilmwindow.ui \
         mainwindow.ui \
         settingswindow.ui \
         splashscreen.ui \
#         statisticswindow.ui

    # Translations #

TRANSLATIONS += ../lang/alexandra-cs.ts \
                ../lang/alexandra-fr.ts \
                ../lang/alexandra-ru_RU.ts \
                ../lang/alexandra-uk_UA.ts

    # Resources #

RESOURCES = icons.qrc \
            images.qrc \
            lang.qrc


    # MediaInfo support #

contains( DEFINES, MEDIAINFO_SUPPORT ) {
    message( "MediaInfo support enabled." )

    HEADERS += filminfowindow.h mediainfo.h
    SOURCES += filminfowindow.cpp mediainfo.cpp
    FORMS += filminfowindow.ui

    linux:LIBS += -lmediainfo
    win32:LIBS += $$PWD\..\lib\MediaInfo.dll
}


    # Portable version #

contains( DEFINES, PORTABLE_VERSION ) {
    message( "Portable version." )
}

    # Debug & Release #

CONFIG(debug, debug|release) {
    message( "DEBUG mode" )

    HEADERS += tools/debug.h
    SOURCES += tools/debug.cpp

    MOC_DIR = ./tmp/debug/moc
    OBJECTS_DIR = ./tmp/debug/obj
    RCC_DIR = ./tmp/debug/rcc
    UI_DIR = ./tmp/debug/ui

} else {
    message( "RELEASE mode" )

    MOC_DIR = ./tmp/release/moc
    OBJECTS_DIR = ./tmp/release/obj
    RCC_DIR = ./tmp/release/rcc
    UI_DIR = ./tmp/release/ui
}

DESTDIR = ./


    # Stylesheets #

RESOURCES += ../styles/alexandraflat-coast/alexandraflat-coast.qrc \
             ../styles/alexandraflat-rainforest/alexandraflat-rainforest.qrc \
             ../styles/qdarkstylesheet/qdarkstylesheet.qrc


    # Installation setup #

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

