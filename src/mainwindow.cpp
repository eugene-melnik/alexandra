#include "mainwindow.h"
#include "version.h"

#include <QProcessEnvironment>
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QFileInfo>
#include <QDir>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
    // Data
    settings = new QSettings( Alexandra::appName, "configuration" );

    // Interface
    setupUi( this );
    setWindowTitle( QString( "%1 v%2" ).arg( Alexandra::appNameGui, Alexandra::appVersionFull ) );
    ConfigureSubwindows();

    LoadSettings();
    SettingsChanged();
}

MainWindow::~MainWindow()
{
    // Subwindows
    delete aboutWindow;
    delete addFilmWindow;
    delete editFilmWindow;
    delete settingsWindow;

    // Variables
    delete settings;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    hide();

    SaveSettings();
    twFilms->SaveDatabase( databaseFileName );

    event->accept();
}

void MainWindow::SettingsChanged()
{
    SetNames();
    twFilms->LoadDatabase( databaseFileName );
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
    bFavourite->setChecked( f->GetIsFavourite() );
    bViewed->setChecked( f->GetIsViewed() );

    QPixmap p = f->GetPoster();

    if( p.isNull() ){
        p.load( ":/standart-poster" );
    }

    p = p.scaledToWidth( lPosterImage->maximumWidth(), Qt::SmoothTransformation );
    lPosterImage->setPixmap( p );

    lTechInformation->setText( f->GetFileName() ); // dummy
}

void MainWindow::PlayFilm()
{
    QProcess player;
    player.startDetached( externalPlayer + " \"" + twFilms->GetCurrentFilmFileName() +"\"" );
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
    connect( actionAboutQt, SIGNAL( triggered() ), aboutWindow, SLOT( AboutQt() ) );

    // Add film window
    addFilmWindow = new AddFilmWindow( this );
    connect( actionAdd, SIGNAL( triggered() ), addFilmWindow, SLOT( show() ) );
    connect( toolbar, SIGNAL( actionAdd() ), addFilmWindow, SLOT( show() ) );

    connect( addFilmWindow, SIGNAL( Done(Film) ), twFilms, SLOT( AppendFilm(Film) ) );

    // Edit film window
    editFilmWindow = new EditFilmWindow( this );
    connect( actionEdit, SIGNAL( triggered() ), this, SLOT( ShowEditFilmWindow() ) );
    connect( toolbar, SIGNAL( actionEdit() ), this, SLOT( ShowEditFilmWindow() ) );

    connect( editFilmWindow, SIGNAL( Done(Film) ), twFilms, SLOT( EditCurrentFilm(Film) ) );

    // Remove film
    connect( actionRemove, SIGNAL( triggered() ), this, SLOT( RemoveFilm() ) );
    connect( toolbar, SIGNAL( actionRemove() ), this, SLOT( RemoveFilm() ) );

    // Settings window
    settingsWindow = new SettingsWindow( settings, this );
    connect( actionSettings, SIGNAL( triggered() ), settingsWindow, SLOT( show() ) );

    connect( settingsWindow, SIGNAL( SettingsChanged() ), this, SLOT( SettingsChanged() ) );

    // Random film
    connect( toolbar, SIGNAL( actionRandom() ), twFilms, SLOT( SelectRandomFilm() ) );
    connect( actionRandom, SIGNAL( triggered() ), twFilms, SLOT( SelectRandomFilm() ) );
}

void MainWindow::SetNames()
{
    /// Set database filename
    databaseFileName = settings->value( "Application/DatabaseFile" ).toString();

    if( databaseFileName.isEmpty() )
    {
#ifdef Q_OS_LINUX
        databaseFileName = QProcessEnvironment::systemEnvironment().value( "XDG_CONFIG_HOME" );

        if( databaseFileName.isEmpty() ) {
            databaseFileName = QProcessEnvironment::systemEnvironment().value( "HOME" ) + "/.config";
        }
#elif defined( Q_OS_WIN32 )
        databaseFileName = QProcessEnvironment::systemEnvironment().value( "APPDATA" );
#endif
        databaseFileName += "/" + Alexandra::appName + "/database.adat";

        settings->setValue( "Application/DatabaseFile", databaseFileName );
        settings->sync();
    }

    // Creating database directory (if not exists)
    QString databaseDir = QFileInfo( databaseFileName ).absolutePath();

    if( !QDir().exists( databaseDir ) ) {
        QDir().mkdir( databaseDir );
    }

    /// Set external player
    externalPlayer = settings->value( "Application/ExternalPlayer" ).toString();

    if( externalPlayer.isEmpty() )
    {
#ifdef Q_OS_LINUX
        externalPlayer = "xdg-open";
#else
        externalPlayer.clear();
#endif
        settings->setValue( "Application/ExternalPlayer", externalPlayer );
        settings->sync();
    }
}

void MainWindow::LoadSettings()
{
    // Main window settings
    restoreGeometry( settings->value( "MainWindow/Geometry" ).toByteArray() );
    restoreState( settings->value( "MainWindow/State" ).toByteArray() );
    actionShowToolbar->setChecked( toolbar->isVisibleTo( this ) );

    // Table settings
    twFilms->LoadSettings( settings );
}

void MainWindow::SaveSettings()
{
    // Main window settings
    settings->setValue( "MainWindow/State", saveState() );
    settings->setValue( "MainWindow/Geometry", saveGeometry() );

    // Table settings
    twFilms->SaveSettings( settings );

    settings->sync();
}
