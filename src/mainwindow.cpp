#include "mainwindow.h"
#include "version.h"

#include <QStandardItemModel>
#include <QTableWidgetItem>
#include <QProcessEnvironment>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
    // Interface
    setupUi( this );
    setWindowTitle( QString( "%1 v%2" ).arg( Alexandra::appNameGui, Alexandra::appVersionFull ) );
    ConfigureSubwindows();
    ConfigureToolbar();

    // Data
    LoadSettings();
    SetDataDirectory();
    films = new FilmsList();
    films->ReadDatabase( dataDirectory );
    FillFilmsTable();
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    SaveSettings();
    films->WriteDatabase( dataDirectory );
    event->accept();
}

void MainWindow::AboutQt()
{
    QMessageBox::aboutQt( this );
}

void MainWindow::ConfigureToolbar()
{
    // Add film
    QAction* tbaAdd = new QAction( QIcon( ":/tool/add" ), tr("Add"), this );
    toolbar->addAction( tbaAdd );
    connect( tbaAdd, SIGNAL( triggered() ), addFilmWindow, SLOT( show() ) );

    // Edit film
    QAction* tbaEdit = new QAction( QIcon( ":/tool/edit" ), tr("Edit"), this );
    toolbar->addAction( tbaEdit );

    // Remove film
    QAction* tbaRemove = new QAction( QIcon( ":/tool/delete" ), tr("Remove"), this );
    toolbar->addAction( tbaRemove );

    toolbar->addSeparator();

    // Random
    QAction* tbaRandom = new QAction( QIcon( ":/tool/random" ), tr("Random"), this );
    toolbar->addAction( tbaRandom );

    // Search
    QAction* tbaSearch = new QAction( QIcon( ":/tool/find" ), tr("Search"), this );
    toolbar->addAction( tbaSearch );

    toolbar->addSeparator();

    // Exit
    QAction* tbaExit = new QAction( QIcon( ":/action/exit" ), tr("Exit"), this );
    toolbar->addAction( tbaExit );
    connect( tbaExit, SIGNAL( triggered() ), this, SLOT( close() ) );
}

void MainWindow::ConfigureSubwindows()
{
    // About and About Qt windows
    aboutWindow = new AboutWindow();
    connect( actionAbout, SIGNAL( triggered() ), aboutWindow, SLOT( show() ) );
    connect( actionAboutQt, SIGNAL( triggered() ), this, SLOT( AboutQt() ) );

    // Add film window
    addFilmWindow = new AddFilmWindow();
    connect( actionAdd, SIGNAL( triggered() ), addFilmWindow, SLOT( show() ) );
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

void MainWindow::FillFilmsTable()
{
    // Configure columns
    QStringList colNames;
    colNames.append( tr( "+" ) );
    colNames.append( tr( "Title" ) );
    colNames.append( tr( "Year" ) );
    colNames.append( tr( "Genre" ) );
    colNames.append( tr( "Director" ) );
    colNames.append( tr( "Rating" ) );

    twFilms->setColumnCount( colNames.size() );
    twFilms->setHorizontalHeaderLabels( colNames );

    twFilms->setColumnWidth( 0, 20 );
    twFilms->setColumnWidth( 1, 150 );
    twFilms->setColumnWidth( 2, 50 );
    twFilms->setColumnWidth( 3, 110 );
    twFilms->setColumnWidth( 4, 110 );
    twFilms->setColumnWidth( 5, 50 );

    // Configure rows
    twFilms->setRowCount( films->size() );

    // Fill table from the database
    for( int row = 0; row != twFilms->rowCount(); row++ )
    {
        // Favourite
        QTableWidgetItem* favourite = new QTableWidgetItem( films->at(row).isFavourite ? "+" : "" );
        twFilms->setItem( row, 0, favourite );

        // Title
        QTableWidgetItem* title = new QTableWidgetItem( films->at(row).title );
        twFilms->setItem( row, 1, title );

        // Year
        QTableWidgetItem* year = new QTableWidgetItem( QString("%1").arg(films->at(row).year) );
        twFilms->setItem( row, 2, year );

        // Genre
        QTableWidgetItem* genre = new QTableWidgetItem( films->at(row).genre );
        twFilms->setItem( row, 3, genre );

        // Director
        QTableWidgetItem* director = new QTableWidgetItem( films->at(row).director );
        twFilms->setItem( row, 4, director );

        // Rating
        QTableWidgetItem* rating = new QTableWidgetItem( QString("%1/10").arg(films->at(row).rating) );
        twFilms->setItem( row, 5, rating);
    }
}
