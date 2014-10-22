#include "mainwindow.h"
#include "version.h"

#include <QStandardItemModel>
#include <QProcessEnvironment>
#include <QMessageBox>
#include <QSettings>

#include<QTableWidget>

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
    event->accept();
}

void MainWindow::AboutQt()
{
    QMessageBox::aboutQt( this );
}

void MainWindow::AddFilm( Film f )
{
    // Scale image
    f.poster = f.poster.scaledToWidth( lPosterImage->maximumWidth(), Qt::SmoothTransformation );
    // And save
    films->append( f );
    films->WriteDatabase( dataDirectory );
}

void MainWindow::FilmSelected( QTableWidgetItem* item )
{
    QString selectedFilmTitle = twFilms->item( item->row(), 1 )->text();
    const Film* f = films->GetFilmByTitle( selectedFilmTitle );

    // Main information
    lFilmTitle->setText( f->title );

    lOriginalTitle->setText( tr( "<b>Original title:</b> %1" ).arg( f->originalTitle ) );
    lTagline->setText( tr( "<b>Tagline:</b> %2" ).arg( f->tagline ) );
    lGenre->setText( tr( "<b>Genre:</b> %3" ).arg( f->genre ) );
    lYear->setText( tr( "<b>Year:</b> %4" ).arg( f->year ) );
    lCountry->setText( tr( "<b>Country:</b> %5" ).arg( f->country ) );
    lDirector->setText( tr( "<b>Director:</b> %6" ).arg( f->director ) );
    lProducer->setText( tr( "<b>Producer:</b> %7" ).arg( f->producer ) );
    lStarring->setText( tr( "<b>Starring:</b> %8" ).arg( f->starring ) );
    lRating->setText( tr( "<b>Rating:</b> %1/10" ).arg( f->rating ) );
    lDescription->setText( tr( "<b>Description:</b> %1" ).arg( f->description ) );
    lPosterImage->setPixmap( f->poster );
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
    // Main window
    connect( twFilms, SIGNAL( itemClicked(QTableWidgetItem*) ), this, SLOT( FilmSelected(QTableWidgetItem*) ) );

    // About and About Qt windows
    aboutWindow = new AboutWindow();
    connect( actionAbout, SIGNAL( triggered() ), aboutWindow, SLOT( show() ) );
    connect( actionAboutQt, SIGNAL( triggered() ), this, SLOT( AboutQt() ) );

    // Add film window
    addFilmWindow = new AddFilmWindow();
    connect( actionAdd, SIGNAL( triggered() ), addFilmWindow, SLOT( show() ) );
    connect( addFilmWindow, SIGNAL( AddFilm(Film) ), this, SLOT( AddFilm(Film) ) );
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

    twFilms->setColumnWidth( 0, s.value( "FilmList/CW0", 20 ).toInt() );
    twFilms->setColumnWidth( 1, s.value( "FilmList/CW1", 150 ).toInt() );
    twFilms->setColumnWidth( 2, s.value( "FilmList/CW2", 50 ).toInt() );
    twFilms->setColumnWidth( 3, s.value( "FilmList/CW3", 110 ).toInt() );
    twFilms->setColumnWidth( 4, s.value( "FilmList/CW4", 110 ).toInt() );
    twFilms->setColumnWidth( 5, s.value( "FilmList/CW5", 50 ).toInt() );
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

    s.setValue( "FilmList/CW0", twFilms->columnWidth(0) );
    s.setValue( "FilmList/CW1", twFilms->columnWidth(1) );
    s.setValue( "FilmList/CW2", twFilms->columnWidth(2) );
    s.setValue( "FilmList/CW3", twFilms->columnWidth(3) );
    s.setValue( "FilmList/CW4", twFilms->columnWidth(4) );
    s.setValue( "FilmList/CW5", twFilms->columnWidth(5) );
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
