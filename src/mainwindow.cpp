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
    hide();

    SaveSettings();
    twFilms->SaveDatabase( dataDirectory );

    event->accept();
}

void MainWindow::AboutQt()
{
    QMessageBox::aboutQt( this );
}

void MainWindow::AddFilm( Film f )
{
    // Scale image
    f.SetPoster( f.GetPoster().scaledToWidth( lPosterImage->maximumWidth(), Qt::SmoothTransformation ) );

    twFilms->AppendFilm( f );
}

void MainWindow::EditFilm( Film f )
{
    // Scale image
    f.SetPoster( f.GetPoster().scaledToWidth( lPosterImage->maximumWidth(), Qt::SmoothTransformation ) );

    twFilms->EditCurrentFilm( f );
}

void MainWindow::ShowEditFilmWindow()
{
    editFilmWindow->show( twFilms->GetCurrentFilm() );
}

void MainWindow::RemoveFilm()
{
    int res = QMessageBox::question( this,
                                     tr( "Remove film" ),
                                     tr( "Are you sure to remove \"%1\"?" ).arg( twFilms->GetCurrentFilmTitle() ),
                                     QMessageBox::Yes, QMessageBox::No );

    if( res == QMessageBox::Yes ) {
        twFilms->RemoveCurrentFilm();
    }
}

void MainWindow::FilmSelected( const Film* f )
{
    // Main information
    lFilmTitle->setText( f->GetTitle() );
    lOriginalTitle->setText( tr( "<b>Original title:</b> %1" ).arg( f->GetOriginalTitle() ) );
    lTagline->setText( tr( "<b>Tagline:</b> %1" ).arg( f->GetTagline() ) );
    lGenre->setText( tr( "<b>Genre:</b> %1" ).arg( f->GetGenre() ) );
    lYear->setText( tr( "<b>Year:</b> %1" ).arg( f->GetYear() ) );
    lCountry->setText( tr( "<b>Country:</b> %1" ).arg( f->GetCountry() ) );
    lDirector->setText( tr( "<b>Director:</b> %1" ).arg( f->GetDirector() ) );
    lProducer->setText( tr( "<b>Producer:</b> %1" ).arg( f->GetProducer() ) );
    lStarring->setText( tr( "<b>Starring:</b> %1" ).arg( f->GetStarring() ) );
    lRating->setText( tr( "<b>Rating:</b> %1" ).arg( f->GetRatingStr() ) );
    lDescription->setText( tr( "<b>Description:</b> %1" ).arg( f->GetDescription() ) );
    lPosterImage->setPixmap( f->GetPoster() );
    bFavourite->setChecked( f->GetIsFavourite() );
    bViewed->setChecked( f->GetIsViewed() );

    lTechInformation->setText( f->GetFileName() ); // dummy
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
    connect( actionAdd, SIGNAL( triggered() ), addFilmWindow, SLOT( show() ) );
    connect( toolbar, SIGNAL( actionAdd() ), addFilmWindow, SLOT( show() ) );

    connect( addFilmWindow, SIGNAL( Done(Film) ), this, SLOT( AddFilm(Film) ) );

    // Edit film window
    editFilmWindow = new EditFilmWindow( this );
    connect( actionEdit, SIGNAL( triggered() ), this, SLOT( ShowEditFilmWindow() ) );
    connect( toolbar, SIGNAL( actionEdit() ), this, SLOT( ShowEditFilmWindow() ) );

    connect( editFilmWindow, SIGNAL( Done(Film) ), this, SLOT( EditFilm(Film) ) );

    // Remove film
    connect( actionRemove, SIGNAL( triggered() ), this, SLOT( RemoveFilm() ) );
    connect( toolbar, SIGNAL( actionRemove() ), this, SLOT( RemoveFilm() ) );

    // Settings window
    settingsWindow = new SettingsWindow( this );
    connect( actionSettings, SIGNAL( triggered() ), settingsWindow, SLOT( show() ) );
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
    restoreGeometry( s.value( "MainWindow/Geometry" ).toByteArray() );
    restoreState( s.value( "MainWindow/State" ).toByteArray() );
    actionShowToolbar->setChecked( toolbar->isVisibleTo( this ) );

    // Table settings
    twFilms->LoadSettings( s );
}

void MainWindow::SaveSettings()
{
    QSettings s( Alexandra::appName, "configuration" );

    // Main window settings
    s.setValue( "MainWindow/State", saveState() );
    s.setValue( "MainWindow/Geometry", saveGeometry() );

    // Table settings
    twFilms->SaveSettings( s );

    s.sync();
}
