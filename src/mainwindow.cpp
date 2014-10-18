#include "mainwindow.h"
#include "version.h"

#include <QStandardItemModel>
#include <QProcessEnvironment>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
    setupUi( this );
    setWindowTitle( QString( "%1 v%2" ).arg( Alexandra::appNameGui, Alexandra::appVersionFull ) );
    SetDataDirectory();

    // subwindows
    aboutWindow = new AboutWindow();
    connect( actionAbout, SIGNAL( triggered() ), aboutWindow, SLOT( show() ) );
    connect( actionAboutQt, SIGNAL( triggered() ), this, SLOT( AboutQt() ) );

    // toolbar
    QAction* tbaAdd = new QAction( QIcon( ":/tool/add" ), tr("Add"), this );
    toolbar->addAction( tbaAdd );

    QAction* tbaEdit = new QAction( QIcon( ":/tool/edit" ), tr("Edit"), this );
    toolbar->addAction( tbaEdit );

    QAction* tbaRemove = new QAction( QIcon( ":/tool/delete" ), tr("Remove"), this );
    toolbar->addAction( tbaRemove );

    toolbar->addSeparator();

    QAction* tbaRandom = new QAction( QIcon( ":/tool/random" ), tr("Random"), this );
    toolbar->addAction( tbaRandom );

    QAction* tbaSearch = new QAction( QIcon( ":/tool/find" ), tr("Search"), this );
    toolbar->addAction( tbaSearch );

    toolbar->addSeparator();

    QAction* tbaExit = new QAction( QIcon( ":/action/exit" ), tr("Exit"), this );
    toolbar->addAction( tbaExit );

    // settings and database
    films.ReadDatabase( dataDirectory );
    LoadSettings();

    // dummy
    lFilmTitle->setText( "Back to the Future 2" );
    lFilmInformation->setText( "<b>Description:</b> After visiting 2015, Marty McFly must repeat his visit to 1955 to prevent disastrous changes to 1985... without interfering with his first trip." );

}

void MainWindow::closeEvent( QCloseEvent* event )
{
    SaveSettings();
    films.WriteDatabase( dataDirectory );

    delete aboutWindow;

    event->accept();
}

void MainWindow::AboutQt()
{
    QMessageBox::aboutQt( this );
}

void MainWindow::SetDataDirectory()
{
#ifdef Q_OS_LINUX
    dataDirectory = QProcessEnvironment::systemEnvironment().value( "XDG_CONFIG_HOME" );

    if( dataDirectory.isEmpty() )
    {
        dataDirectory = QProcessEnvironment::systemEnvironment().value( "HOME" ) + "/.config";
    }
#endif

    dataDirectory.append( QString("/") + Alexandra::appName + QString("/") );
}

void MainWindow::LoadSettings()
{
    QSettings s( Alexandra::appName, "configuration" );

    restoreState( s.value( "MainWindow/State" ).toByteArray() );
    move( s.value( "MainWindow/Position" ).toPoint() );

    if( s.value( "MainWindow/Maximized", false ).toBool() )
    {
        showMaximized();
    }
    else
    {
        resize( s.value( "MainWindow/Size" ).toSize() );
    }

    actionShowToolbar->setChecked( s.value( "MainWindow/ShowToolbar", true ).toBool() );
    toolbar->move( s.value( "MainWindow/ToolbarPosition" ).toPoint() );
}

void MainWindow::SaveSettings()
{
    QSettings s( Alexandra::appName, "configuration" );

    s.setValue( "MainWindow/State", saveState() );
    s.setValue( "MainWindow/Position", pos() );
    s.setValue( "MainWindow/Size", size() );
    s.setValue( "MainWindow/Maximized", isMaximized() );
    s.setValue( "MainWindow/ShowToolbar", toolbar->isVisible() );
    s.setValue( "MainWindow/ToolbarPosition", toolbar->pos() );
}
