#include "alexandra.h"
#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QTime>

int main( int argc, char** argv )
{
    // Create seed for the random
    qsrand( QTime::currentTime().msec() );

    // Configure application
    QApplication::setOrganizationName( Alexandra::orgName );
    QApplication::setOrganizationDomain( Alexandra::orgDomain );
    QApplication::setApplicationName( Alexandra::appNameGui );

    QApplication alexandra( argc, argv );

    QTranslator* translator = new QTranslator();
    translator->load( ":/lang/alexandra-locale.qm" );
    alexandra.installTranslator( translator );
    QTranslator* qt_translator = new QTranslator();
    qt_translator->load( ":/lang/qt-locale.qm" );
    alexandra.installTranslator( qt_translator );

    // Run
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
    return( alexandra.exec() );
}
