/*************************************************************************************************
 *                                                                                                *
 *  file: mainwindow.cpp                                                                          *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>                                     *
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

#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPoint>
#include <QPushButton>
#include <QStatusBar>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
    // Interface
    setupUi( this );
    setWindowTitle( Alexandra::appNameGui );
    eFilter->setFocus();

    // Data
    settings = new AlexandraSettings( this );
    externalPlayer = new QProcess( this );

    ConfigureSubwindows();
    LoadSettings();

    twFilms->LoadDatabase();
}

MainWindow::~MainWindow()
{
    // Subwindows
    delete aboutWindow;
    delete addFilmWindow;
    delete editFilmWindow;
    delete settingsWindow;
    delete splashScreen;

    // Variables
    delete settings;
    delete externalPlayer;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    hide();
    SaveSettings();
    event->accept();
}

void MainWindow::SettingsChanged()
{
    QApplication::setStyle( settings->GetApplicationStyle() );
    toolbar->LoadSettings( settings );
    twFilms->LoadSettings( settings );
}

void MainWindow::DatabaseSettingsChanged()
{
    twFilms->LoadDatabase();
}

void MainWindow::DatabaseChanged()
{
    twFilms->SaveDatabase();
    SetAllFunctionsEnabled( true );
    UpdateStatusBar();
}

void MainWindow::DatabaseReadError()
{
    SetAllFunctionsEnabled( false );
    ClearTextFields();
    lFilmTitle->setText( tr( "Error reading the database!" ) );

    QMessageBox::critical( this,
                           tr( "Database" ),
                           tr( "Error reading the database! Check the permissions or choose another "
                               "database file in \"Edit\"→\"Settings\"→\"Database\"." ) );
}

void MainWindow::DatabaseIsEmpty()
{
    SetEmptyMode();

    ClearTextFields();
    lFilmTitle->setText( tr( "Database is empty!" ) );

    QMessageBox::information( this,
                              tr( "Database is empty!"),
                              tr( "You can add your films in menu \"Films\"→\"Add film\" or choose "
                                  "an another database in \"Edit\"→\"Settings\"→\"Database\"." ) );
}

void MainWindow::DatabaseIsReadonly()
{
    SetReadOnlyMode();

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
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

    if( res == QMessageBox::Yes ) {
        twFilms->RemoveCurrentFilm();
    }
}

void MainWindow::RemoveFile()
{
    int res = QMessageBox::question( this,
                                     tr( "Remove file" ),
                                     tr( "Are you sure to remove file \"%1\"?" ).arg( twFilms->GetCurrentFilmFileName() ),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

    if( res == QMessageBox::Yes ) {
        if( QFile( twFilms->GetCurrentFilmFileName() ).remove() == true ) {
            twFilms->RemoveCurrentFilm();
        } else {
            QMessageBox::warning( this,
                                  tr( "Remove file" ),
                                  tr( "Unable to remove file \"%1\"!" ).arg( twFilms->GetCurrentFilmFileName() ) );
        }
    }
}

void MainWindow::ShowFilmContextMenu( QPoint p )
{
    QMenu contextMenu;

    // Play
    contextMenu.addAction( QIcon( ":/action/play"), tr( "Play" ), this, SLOT( PlayFilm() ) );
    // Separator
    contextMenu.addSeparator();
    // IsViewed
    QAction* a = contextMenu.addAction( tr( "Is viewed" ) );
    a->setCheckable( true );
    a->setChecked( twFilms->GetCurrentFilm()->GetIsViewed() );
    connect( a, SIGNAL( toggled(bool) ), twFilms, SLOT( SetCurrentIsViewed(bool) ) );
    // IsFavourite
    a = contextMenu.addAction( tr( "Is favourite" ) );
    a->setCheckable( true );
    a->setChecked( twFilms->GetCurrentFilm()->GetIsFavourite() );
    connect( a, SIGNAL( toggled(bool) ), twFilms, SLOT( SetCurrentIsFavourite(bool) ) );
    // Separator
    contextMenu.addSeparator();
    // Edit
    contextMenu.addAction( QIcon( ":/tool/edit" ), tr( "Edit" ), this, SLOT( ShowEditFilmWindow() ) );
    // Remove
    contextMenu.addAction( QIcon( ":/tool/delete" ), tr( "Remove" ), this, SLOT( RemoveFilm() ) );
    // Separator
    contextMenu.addSeparator();
    // Remove file
    contextMenu.addAction( tr( "Remove file" ), this, SLOT( RemoveFile() ) );

    QPoint pos = twFilms->viewport()->mapToGlobal( p );
    contextMenu.exec( pos );
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
    lTags->setText( tr( "<b>Tags:</b> %1" ).arg( f->GetTags() ) );

    bViewed->setChecked( f->GetIsViewed() );
    bFavourite->setChecked( f->GetIsFavourite() );
    lTechInformation->setText( "—" );

    // Poster
    QPixmap p( settings->GetFilmsListPostersDir() + "/" + f->GetPosterName() );

    if( p.isNull() ) {
        p.load( ":/standart-poster" );
    }

    p = p.scaledToWidth( lPosterImage->maximumWidth(), Qt::SmoothTransformation );
    lPosterImage->setPixmap( p );

    // Buttons and technical information
    bTechInformation->setEnabled( false );

    if( QFileInfo( f->GetFileName() ).exists() ) {
        // File exists
        filmInfoWindow->SetCurrentFile( f->GetFileName() );
        bPlay->setEnabled( true );
    } else {
        // File doesn't exists
        bPlay->setEnabled( false );
    }
}

void MainWindow::ShowShortInfo( QString s )
{
    lTechInformation->setText( s );
    bTechInformation->setEnabled( true );
}

void MainWindow::PlayFilm()
{
    if( externalPlayer->state() == QProcess::NotRunning ) {
#ifdef Q_OS_WIN32
        externalPlayer->start( "\"" + settings->GetApplicationExternalPlayer() + "\" \"" + twFilms->GetCurrentFilmFileName() +"\"" );
#else
        externalPlayer->start( settings->GetApplicationExternalPlayer() + " \"" + twFilms->GetCurrentFilmFileName() +"\"" );
#endif
    } else {
        externalPlayer->close();
    }
}

void MainWindow::PlayerStarted()
{
    twFilms->setEnabled( false );
    eFilter->setEnabled( false );
    bPlay->setText( tr( "STOP" ) );
}

void MainWindow::PlayerClosed()
{
    twFilms->setEnabled( true );
    eFilter->setEnabled( true );
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
    // Splashscreen
    splashScreen = new SplashScreen();
    splashScreen->show();
    connect( twFilms, SIGNAL( DatabaseChanged() ), splashScreen, SLOT( Close() ) );

    // Main window
    connect( toolbar, SIGNAL( actionExit() ), this, SLOT( close() ) );

    connect( twFilms, SIGNAL( DatabaseReadError() ), this, SLOT( DatabaseReadError() ) );
    connect( twFilms, SIGNAL( DatabaseIsReadonly() ), this, SLOT( DatabaseIsReadonly() ) );
    connect( twFilms, SIGNAL( DatabaseIsEmpty() ), this, SLOT( DatabaseIsEmpty() ) );
    connect( twFilms, SIGNAL( DatabaseChanged() ), this, SLOT( DatabaseChanged() ) );
    connect( twFilms, SIGNAL( FilmSelected(const Film*) ), this, SLOT( FilmSelected(const Film*) ) );
    connect( twFilms, SIGNAL( itemDoubleClicked(QTableWidgetItem*) ), this, SLOT( PlayFilm() ) );
    connect( twFilms, SIGNAL( customContextMenuRequested(QPoint) ), this, SLOT( ShowFilmContextMenu(QPoint) ) );

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

    // FilmInfo window
    filmInfoWindow = new FilmInfoWindow( this );
    connect( bTechInformation, SIGNAL( clicked() ), filmInfoWindow, SLOT( show() ) );

    connect( filmInfoWindow, SIGNAL( ShortInfoLoaded(QString) ), this, SLOT( ShowShortInfo(QString) ) );

    // Remove film
    connect( actionRemove, SIGNAL( triggered() ), this, SLOT( RemoveFilm() ) );
    connect( toolbar, SIGNAL( actionRemove() ), this, SLOT( RemoveFilm() ) );

    // Search window
    searchWindow = new SearchWindow( twFilms->GetFilmsList(), this );
    connect( actionSearch, SIGNAL( triggered() ), searchWindow, SLOT( show() ) );
    connect( toolbar, SIGNAL( actionSearch() ), searchWindow, SLOT( show() ) );

    connect( searchWindow, SIGNAL( FilmSelected(QString) ), twFilms, SLOT( SelectFilm(QString) ) );

    // Settings window
    settingsWindow = new SettingsWindow( settings, this );
    connect( actionSettings, SIGNAL( triggered() ), settingsWindow, SLOT( show() ) );

    connect( settingsWindow, SIGNAL( SettingsChanged() ), this, SLOT( SettingsChanged() ) );
    connect( settingsWindow, SIGNAL( DatabaseSettingsChanged() ), this, SLOT( DatabaseSettingsChanged() ) );
    connect( settingsWindow, SIGNAL( EraseDatabase() ), twFilms, SLOT( EraseDatabase() ) );

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
    lTags->clear();
    lTechInformation->clear();
    repaint(); // Need for removing the artifacts
}

void MainWindow::LoadSettings()
{
    QApplication::setStyle( settings->GetApplicationStyle() );

    // Main window settings
    restoreGeometry( settings->GetMainWindowGeometry() );
    restoreState( settings->GetMainWindowState() );
    mainSplitter->restoreState( settings->GetMainWindowSplitterState() );
    actionShowToolbar->setChecked( toolbar->isVisibleTo( this ) );

    // Widgets' settings
    toolbar->LoadSettings( settings );
    twFilms->LoadSettings( settings );
}

void MainWindow::SaveSettings()
{
    // Main window settings
    settings->SetMainWindowState( saveState() );
    settings->SetMainWindowGeometry( saveGeometry() );
    settings->SetMainWindowSplitterState( mainSplitter->saveState() );

    // Table settings
    twFilms->SaveSettings();

    settings->sync();
}

void MainWindow::SetAllFunctionsEnabled( bool b )
{
    toolbar->SetAllFunctionsEnabled( b );

    actionAdd->setEnabled( b );
    actionEdit->setEnabled( b );
    actionRemove->setEnabled( b );
    actionRandom->setEnabled( b );
    actionSearch->setEnabled( b );

    bViewed->setEnabled( b );
    bFavourite->setEnabled( b );
    bTechInformation->setEnabled( b );
    bPlay->setEnabled( b );
}

void MainWindow::SetEmptyMode( bool b )
{
    toolbar->SetEmptyDatabaseMode( b );

    actionEdit->setDisabled( b );
    actionRemove->setDisabled( b );
    actionRandom->setDisabled( b );
    actionSearch->setDisabled( b );

    bViewed->setDisabled( b );
    bFavourite->setDisabled( b );
    bTechInformation->setDisabled( b );
    bPlay->setDisabled( b );
}

void MainWindow::SetReadOnlyMode( bool b )
{
    toolbar->SetReadOnlyMode( b );

    actionAdd->setDisabled( b );
    actionEdit->setDisabled( b );
    actionRemove->setDisabled( b );

    bViewed->setDisabled( b );
    bFavourite->setDisabled( b );
}
