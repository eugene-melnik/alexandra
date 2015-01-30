TARGET = alexandra
TEMPLATE = app

QT = core gui widgets concurrent

CONFIG += qt warn_on debug
QMAKE_CXXFLAGS += -std=c++11
DEFINES += UNICODE
LIBS += -lmediainfo -lzen

#release {
#  QMAKE_CXXFLAGS += -flto
#}

HEADERS += aboutwindow.h \
           addfilmwindow.h \
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

