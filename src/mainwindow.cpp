/*************************************************************************************************
 *                                                                                                *
 *  file: mainwindow.cpp                                                                          *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014 Eugene Melnik <jeka7js@gmail.com>                                          *
 *                                                                                                *
 *  Alexandra is free software; you can redistribute it and/or modify it under the terms of the   *
 *  GNU General Public License as published by the Free Software Foundation; either version 2 of  *
 *  the License, or (at your option) any later version.                                           *
 *                                                                                                *
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     *
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     *
 *  See the GNU General Public License for more details.                                          *
 *                                                                                                *
 *  You should have received a copy of the GNU General Public License along with this program.    *
 *  If not, see <http://www.gnu.org/licenses/>.                                                   *
 *                                                                                                *
  *************************************************************************************************/

#include "mainwindow.h"
#include "version.h"

#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QPushButton>
#include <QSettings>
#include <QStatusBar>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
    // Interface
    setupUi( this );
    setWindowTitle( QString( "%1 v%2" ).arg( Alexandra::appNameGui, Alexandra::appVersionFull ) );
    eFilter->setFocus();

    // Data
    settings = new QSettings( Alexandra::appName, "configuration" );
    externalPlayer = new QProcess( this );

    ConfigureSubwindows();
    LoadSettings();
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
    delete externalPlayer;
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
    toolbar->LoadSettings( settings );
}

void MainWindow::DatabaseSettingsChanged()
{
    twFilms->LoadDatabase( databaseFileName );
}

void MainWindow::DatabaseChanged()
{
    SetEditFunctionsEnabled( true );
    UpdateStatusBar();
}

void MainWindow::DatabaseReadError()
{
    ClearTextFields();
    lFilmTitle->setText( tr( "Error reading the database!" ) );

    QMessageBox::critical( this,
                           tr( "Database" ),
                           tr( "Error reading the database! Check the permissions or choose another "
                               "database file in \"Edit\"→\"Settings\"→\"Database\"." ) );
}

void MainWindow::DatabaseIsReadonly()
{
    SetEditFunctionsEnabled( false );

    QMessageBox::information( this,
                              tr( "Database" ),
                              tr( "Database is readonly! Editing functions are disabled." ) );
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

void MainWindow::ShowFirstStepWizard()
{
    ClearTextFields();
    lFilmTitle->setText( tr( "Database is empty!" ) );

    QMessageBox::information( this,
                              tr( "Database is empty!"),
                              tr( "You can add your films in menu \"Films\"→\"Add film\" or choose "
                                  "an another database in \"Edit\"→\"Settings\"→\"Database\"." ) );
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

    bViewed->setChecked( f->GetIsViewed() );
    bFavourite->setChecked( f->GetIsFavourite() );

    // Poster
    QPixmap p( postersFolderName + "/" + f->GetPosterName() );

    if( p.isNull() ) {
        p.load( ":/standart-poster" );
    }

    p = p.scaledToWidth( lPosterImage->maximumWidth(), Qt::SmoothTransformation );
    lPosterImage->setPixmap( p );

    // Technical information
    lTechInformation->setText( QString( "%1GiB" ).arg( QFileInfo( f->GetFileName() ).size() / (1024*1024*1024) ) ); // dummy

    // Play button
    bPlay->setEnabled( QFileInfo( f->GetFileName() ).exists() );
}

void MainWindow::PlayFilm()
{
    if( externalPlayer->state() == QProcess::NotRunning ) {
        externalPlayer->start( externalPlayerName + " \"" + twFilms->GetCurrentFilmFileName() +"\"" );
    } else {
        externalPlayer->close();
    }
}

void MainWindow::PlayerStarted()
{
    twFilms->setEnabled( false );
    bPlay->setText( tr( "STOP" ) );
}

void MainWindow::PlayerClosed()
{
    twFilms->setEnabled( true );
    bPlay->setText( tr( "PLAY" ) );

    if( bViewed->isEnabled() && !bViewed->isChecked() ) {
        bViewed->setChecked( true );
        bViewed->clicked( true );
    }
}

void MainWindow::UpdateStatusBar()
{
    statusbar->Show( twFilms->GetNumberOfFilms(),
                     twFilms->GetIsViewedCount(),
                     twFilms->GetIsFavouriteCount() );
}

void MainWindow::ConfigureSubwindows()
{
    // Main window
    connect( toolbar, SIGNAL( actionExit() ), this, SLOT( close() ) );

    connect( twFilms, SIGNAL( DatabaseReadError() ), this, SLOT( DatabaseReadError() ) );
    connect( twFilms, SIGNAL( DatabaseIsReadonly() ), this, SLOT( DatabaseIsReadonly() ) );
    connect( twFilms, SIGNAL( DatabaseIsEmpty() ), this, SLOT( ShowFirstStepWizard() ) );
    connect( twFilms, SIGNAL( DatabaseChanged() ), this, SLOT( DatabaseChanged() ) );
    connect( twFilms, SIGNAL( FilmSelected(const Film*) ), this, SLOT( FilmSelected(const Film*) ) );

    connect( eFilter, SIGNAL( textChanged(QString) ), twFilms, SLOT( FilterBy(QString) ) );

    connect( bViewed, SIGNAL( clicked(bool) ), twFilms, SLOT( SetCurrentIsViewed(bool) ) );
    connect( bFavourite, SIGNAL( clicked(bool) ), twFilms, SLOT( SetCurrentIsFavourite(bool) ) );
    connect( bPlay, SIGNAL( clicked() ), this, SLOT( PlayFilm() ) );

    connect( externalPlayer, SIGNAL( started() ), this, SLOT( PlayerStarted() ) );
    connect( externalPlayer, SIGNAL( finished(int) ), this, SLOT( PlayerClosed() ) );

    // About and About Qt windows
    aboutWindow = new AboutWindow( this );
    connect( actionAbout, SIGNAL( triggered() ), aboutWindow, SLOT( show() ) );
    connect( actionAboutQt, SIGNAL( triggered() ), aboutWindow, SLOT( AboutQt() ) );

    // Add film window
    addFilmWindow = new AddFilmWindow( settings, this );
    connect( actionAdd, SIGNAL( triggered() ), addFilmWindow, SLOT( show() ) );
    connect( toolbar, SIGNAL( actionAdd() ), addFilmWindow, SLOT( show() ) );

    connect( addFilmWindow, SIGNAL( Done(Film) ), twFilms, SLOT( AppendFilm(Film) ) );

    // Edit film window
    editFilmWindow = new EditFilmWindow( settings, this );
    connect( actionEdit, SIGNAL( triggered() ), this, SLOT( ShowEditFilmWindow() ) );
    connect( toolbar, SIGNAL( actionEdit() ), this, SLOT( ShowEditFilmWindow() ) );

    connect( editFilmWindow, SIGNAL( Done(Film) ), twFilms, SLOT( EditCurrentFilm(Film) ) );

    // Remove film
    connect( actionRemove, SIGNAL( triggered() ), this, SLOT( RemoveFilm() ) );
    connect( toolbar, SIGNAL( actionRemove() ), this, SLOT( RemoveFilm() ) );

    // Search window
    searchWindow = new SearchWindow( this );
    connect( actionSearch, SIGNAL( triggered() ), searchWindow, SLOT( show() ) );
    connect( toolbar, SIGNAL( actionSearch() ), searchWindow, SLOT( show() ) );

    // Settings window
    settingsWindow = new SettingsWindow( settings, this );
    connect( actionSettings, SIGNAL( triggered() ), settingsWindow, SLOT( show() ) );

    connect( settingsWindow, SIGNAL( SettingsChanged() ), this, SLOT( SettingsChanged() ) );
    connect( settingsWindow, SIGNAL( DatabaseSettingsChanged() ), this, SLOT( DatabaseSettingsChanged() ) );

    // Random film
    connect( toolbar, SIGNAL( actionRandom() ), twFilms, SLOT( SelectRandomFilm() ) );
    connect( actionRandom, SIGNAL( triggered() ), twFilms, SLOT( SelectRandomFilm() ) );
}

void MainWindow::ClearTextFields()
{
    lFilmTitle->clear();
    lPosterImage->setPixmap( QPixmap( ":/standart-poster" ).scaledToWidth( lPosterImage->maximumWidth(), Qt::SmoothTransformation ) );
    lOriginalTitle->clear();
    lTagline->clear();
    lGenre->clear();
    lYear->clear();
    lCountry->clear();
    lDirector->clear();
    lProducer->clear();
    lStarring->clear();
    lRating->clear();
    lDescription->clear();
    lTechInformation->clear();
    repaint(); // Need for removing the artifacts
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
    }

    // Creating database directory and database file (if not exists)
    QString databaseDir = QFileInfo( databaseFileName ).absolutePath();

    if( !QDir().exists( databaseDir ) ) {
        QDir().mkdir( databaseDir );
    }

    if( !QFileInfo( databaseFileName ).exists() ) {
        QFile f( databaseFileName );
        f.open( QIODevice::WriteOnly );
        f.close();
    }

    /// Set posters' folder name
    postersFolderName = settings->value( "FilmList/PostersFolder" ).toString();

    if( postersFolderName.isEmpty() ) {
        postersFolderName = databaseDir + "/posters";
        settings->setValue( "FilmList/PostersFolder", postersFolderName );
    }

    // Creating posters' directory (if not exists)
    if( !QDir().exists( postersFolderName ) ) {
        QDir().mkdir( postersFolderName );
    }

    /// Set external player
    externalPlayerName = settings->value( "Application/ExternalPlayer" ).toString();

    if( externalPlayerName.isEmpty() )
    {
#ifdef Q_OS_LINUX
        externalPlayerName = "xdg-open";
#else
        externalPlayer.clear();
#endif
        settings->setValue( "Application/ExternalPlayer", externalPlayerName );
    }

    settings->sync();
}

void MainWindow::LoadSettings()
{
    QApplication::setStyle( settings->value( "Application/Style" ).toString() );

    // Main window settings
    SetNames();

    restoreGeometry( settings->value( "MainWindow/Geometry" ).toByteArray() );
    restoreState( settings->value( "MainWindow/State" ).toByteArray() );
    actionShowToolbar->setChecked( toolbar->isVisibleTo( this ) );

    // Widgets' settings
    toolbar->LoadSettings( settings );
    twFilms->LoadSettings( settings );
    twFilms->LoadDatabase( databaseFileName );
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

void MainWindow::SetEditFunctionsEnabled( bool b )
{
    toolbar->SetEditFunctionsEnabled( b );

    actionAdd->setEnabled( b );
    actionEdit->setEnabled( b );
    actionRemove->setEnabled( b );

    bViewed->setEnabled( b );
    bFavourite->setEnabled( b );
}
