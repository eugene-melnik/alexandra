#include "mainwindow.h"
#include "version.h"

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
    setupUi( this );
    setWindowTitle( QString( "%1 v%2" ).arg( Alexandra::appNameGui, Alexandra::appVersionFull ) );

    aboutWindow = new AboutWindow();
    connect( actionAbout, SIGNAL( triggered() ), aboutWindow, SLOT( show() ) );
}

MainWindow::~MainWindow()
{
    delete aboutWindow;
}
