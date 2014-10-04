#include "mainwindow.h"
#include "version.h"

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
    setupUi( this );
    setWindowTitle( QString( "%1 v%2" ).arg( Alexandra::appNameGui, Alexandra::appVersionFull ) );

    // subwindows
    aboutWindow = new AboutWindow();
    connect( actionAbout, SIGNAL( triggered() ), aboutWindow, SLOT( show() ) );

    // toolbar
    QAction* tbaAdd = new QAction( tr("Add"), this );
    toolbar->addAction( tbaAdd );

    QAction* tbaEdit = new QAction( tr("Edit"), this );
    toolbar->addAction( tbaEdit );

    QAction* tbaRemove = new QAction( tr("Remove"), this );
    toolbar->addAction( tbaRemove );

    toolbar->addSeparator();

    QAction* tbaRandom = new QAction( tr("Random"), this );
    toolbar->addAction( tbaRandom );

    QAction* tbaSearch = new QAction( tr("Search"), this );
    toolbar->addAction( tbaSearch );

    toolbar->addSeparator();

    QAction* tbaExit = new QAction( tr("Exit"), this );
    toolbar->addAction( tbaExit );

    // dummy
    lFilmTitle->setText( "Back to the Future 2" );
    lFilmInformation->setText( "<b>Description:</b> After visiting 2015, Marty McFly must repeat his visit to 1955 to prevent disastrous changes to 1985... without interfering with his first trip." );

}

MainWindow::~MainWindow()
{
    delete aboutWindow;
}
