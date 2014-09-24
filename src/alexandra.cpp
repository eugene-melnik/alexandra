#include "alexandra.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main( int argc, char** argv )
{
    QApplication::setOrganizationName( Alexandra::orgName );
    QApplication::setOrganizationDomain( Alexandra::orgDomain );
    QApplication::setApplicationName( Alexandra::appNameGui );

    QApplication alexandra( argc, argv );

    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();

    return( alexandra.exec() );
}
