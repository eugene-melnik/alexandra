#include "mainwindow.h"
#include "version.h"

#include <QProcessEnvironment>
#include <QMessageBox>
#include <QSettings>
#include <QProcess>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
    // Interface
    setupUi( this );
    setWindowTitle( QString( "%1 v%2" ).arg( Alexandra::appNameGui, Alexandra::appVersionFull ) );
    ConfigureSubwindows();

    // Data
    LoadSettings();
    SetDataDirectory();
    twFilms->LoadDatabase( dataDirectory );
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
    twFilms->AppendFilm( f );
    twFilms->SaveDatabase( dataDirectory );
}

void MainWindow::RemoveFilm()
{
    int res = QMessageBox::question( this,
                                     tr( "Remove film" ),
                                     tr( "Are you sure to remove \"%1\"?" ).arg( twFilms->GetCurrentFilmTitle() ),
                                     QMessageBox::Yes, QMessageBox::No );

    if( res == QMessageBox::Yes) {
        //delete
    }
}

void MainWindow::FilmSelected( const Film* f )
{
    // Main information
    lFilmTitle->setText( f->title );
    lOriginalTitle->setText( tr( "<b>Original title:</b> %1" ).arg( f->originalTitle ) );
    lTagline->setText( tr( "<b>Tagline:</b> %1" ).arg( f->tagline ) );
    lGenre->setText( tr( "<b>Genre:</b> %1" ).arg( f->genre ) );
    lYear->setText( tr( "<b>Year:</b> %1" ).arg( f->year ) );
    lCountry->setText( tr( "<b>Country:</b> %1" ).arg( f->country ) );
    lDirector->setText( tr( "<b>Director:</b> %1" ).arg( f->director ) );
    lProducer->setText( tr( "<b>Producer:</b> %1" ).arg( f->producer ) );
    lStarring->setText( tr( "<b>Starring:</b> %1" ).arg( f->starring ) );
    lRating->setText( tr( "<b>Rating:</b> %1/10" ).arg( f->rating ) );
    lDescription->setText( tr( "<b>Description:</b> %1" ).arg( f->description ) );
    lPosterImage->setPixmap( f->poster );
    bFavourite->setChecked( f->isFavourite );
    bViewed->setChecked( f->isViewed );

    lTechInformation->setText( f->fileName ); // dummy
}

void MainWindow::PlayFilm()
{
    QProcess player;
    player.startDetached( "xdg-open \"" + twFilms->GetCurrentFilmFileName() +"\"" );
}

void MainWindow::UpdateStatusBar()
{
    statusbar->showMessage( tr( "Total films: %1 (%2 viewed, %3 favourite)" )
                            .arg( twFilms->GetNumberOfFilms() )
                            .arg( twFilms->GetIsViewedCount() )
                            .arg( twFilms->GetIsFavouriteCount() ) );
}

void MainWindow::ConfigureSubwindows()
{
    // Main window
    connect( toolbar, SIGNAL( actionExit() ), this, SLOT( close() ) );
    connect( twFilms, SIGNAL( DatabaseChanged() ), this, SLOT( UpdateStatusBar() ) );
    connect( twFilms, SIGNAL( FilmSelected(const Film*) ), this, SLOT( FilmSelected(const Film*) ) );
    connect( bPlay, SIGNAL( clicked() ), this, SLOT( PlayFilm() ) );

    // About and About Qt windows
    aboutWindow = new AboutWindow( this );
    connect( actionAbout, SIGNAL( triggered() ), aboutWindow, SLOT( show() ) );
    connect( actionAboutQt, SIGNAL( triggered() ), this, SLOT( AboutQt() ) );

    // Add film window
    addFilmWindow = new AddFilmWindow( this );
    connect( toolbar, SIGNAL( actionAdd() ), addFilmWindow, SLOT( show() ) );
    connect( actionAdd, SIGNAL( triggered() ), addFilmWindow, SLOT( show() ) );
    connect( addFilmWindow, SIGNAL( AddFilm(Film) ), this, SLOT( AddFilm(Film) ) );

    // Remove film
    connect( actionRemove, SIGNAL( triggered() ), this, SLOT( RemoveFilm() ) );
    connect( toolbar, SIGNAL( actionRemove() ), this, SLOT( RemoveFilm() ) );
}

void MainWindow::SetDataDirectory()
{
#ifdef Q_OS_LINUX
    dataDirectory = QProcessEnvironment::systemEnvironment().value( "XDG_CONFIG_HOME" );

    if( dataDirectory.isEmpty() ) {
        dataDirectory = QProcessEnvironment::systemEnvironment().value( "HOME" ) + "/.config";
    }
#endif

    dataDirectory.append( QString("/") + Alexandra::appName + QString("/") );
}

void MainWindow::LoadSettings()
{
    QSettings s( Alexandra::appName, "configuration" );

    // Main window settings
    restoreState( s.value( "MainWindow/State" ).toByteArray() );
    move( s.value( "MainWindow/Position" ).toPoint() );

    if( s.value( "MainWindow/Maximized", false ).toBool() ) {
        showMaximized();
    } else {
        resize( s.value( "MainWindow/Size" ).toSize() );
    }

    actionShowToolbar->setChecked( s.value( "MainWindow/ShowToolbar", true ).toBool() );
    toolbar->move( s.value( "MainWindow/ToolbarPosition" ).toPoint() );

    // Table settings
    twFilms->LoadSettings( s );
}

void MainWindow::SaveSettings()
{
    QSettings s( Alexandra::appName, "configuration" );

    // Main window settings
    s.setValue( "MainWindow/State", saveState() );
    s.setValue( "MainWindow/Position", pos() );
    s.setValue( "MainWindow/Size", size() );
    s.setValue( "MainWindow/Maximized", isMaximized() );
    s.setValue( "MainWindow/ShowToolbar", toolbar->isVisible() );
    s.setValue( "MainWindow/ToolbarPosition", toolbar->pos() );

    // Table settings
    twFilms->SaveSettings( s );
}
