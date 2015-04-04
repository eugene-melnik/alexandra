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

#include "filmsviewgrid.h"
#include "filmsviewlist.h"
#include "mainwindow.h"
#include "version.h"

#include <QColor>
#include <QFileInfo>
#include <QLineEdit>
#include <QMessageBox>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
    // Interface
    setupUi( this );
    setWindowTitle( Alexandra::appNameGui );
    eFilter->setFocus();

    // Data
    settings = new AlexandraSettings( this );
    externalPlayer = new QProcess( this );
    filmsList = new FilmsList( settings, this );

    SetupFilmsView();
    SetupWindows();
    LoadSettings();

    filmsList->LoadFromFile( settings->GetDatabaseFilePath() ); // FIXME: double ShowFilmInformation()
    filmsView->SelectItem( settings->GetCurrentFilmTitle() );   //        call at startup
}

MainWindow::~MainWindow()
{
    // Child windows
    delete aboutWindow;
    delete editFilmWindow;
    delete filmInfoWindow;
    delete searchWindow;
    delete settingsWindow;
    delete splashScreen;

    // Variables
    delete settings;
    delete externalPlayer;
    delete filmsList;
    delete filmsView;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    hide();
    SaveSettings();
    event->accept();
}

void MainWindow::SaveDatabase()
{
    filmsList->SaveToFileAsync( settings->GetDatabaseFilePath() );
}

void MainWindow::ReloadDatabase()
{
    filmsList->LoadFromFile( settings->GetDatabaseFilePath() );
    filmsView->SelectItem( settings->GetCurrentFilmTitle() );
}

void MainWindow::ReloadSettings()
{
    QApplication::setStyle( settings->GetApplicationStyleName() );
    filmsView->ReloadSettings( settings );
    toolbar->LoadSettings( settings );
    // TODO: right panel on/off
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

void MainWindow::ShowFilms()
{
    statusbar->ShowLoading();

    bool highlightUnavailable = settings->GetCheckFilesOnStartup();
    QColor unavailableColor = settings->GetUnavailableFileColor();

    int currentIndex = filmsView->GetCurrentItemIndex();
    filmsView->Clear();

    const QList<Film>* films = filmsList->GetFilmsList();

    for( int i = 0; i < films->size(); i++ )
    {
        const Film& film = films->at(i);

        if( highlightUnavailable && !QFileInfo( film.GetFileName() ).exists() )
        {
            filmsView->AddItem( film, unavailableColor );
        }
        else
        {
            filmsView->AddItem( film );
        }
    }

    statusbar->ShowTotal( filmsList->GetFilmsCount(), filmsList->GetIsViewedCount(), filmsList->GetIsFavouriteCount() );
    filmsView->SetCurrentItemIndex( currentIndex );
    emit Shown();
}

void MainWindow::ShowFilmInformation()
{
    const Film* f = filmsList->GetCurrentFilm();

    // Buttons and technical information
    bTechInformation->setEnabled( false );

    if( QFileInfo( f->GetFileName() ).exists() ) {
        // File exists
        filmInfoWindow->LoadTechnicalInfoAsync( f->GetFileName() );
        bPlay->setEnabled( true );
    } else {
        // File doesn't exists
        bPlay->setEnabled( false );
    }

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
    QPixmap p( settings->GetPostersDirPath() + "/" + f->GetPosterName() );

    if( p.isNull() ) {
        p.load( ":/standart-poster" );
    }

    p = p.scaledToWidth( lPosterImage->maximumWidth(), Qt::SmoothTransformation );
    lPosterImage->setPixmap( p );
}

void MainWindow::ShowShortTechnicalInfo( QString info )
{
    lTechInformation->setText( info );
    bTechInformation->setEnabled( true );
}

void MainWindow::PlayFilm()
{
    if( externalPlayer->state() == QProcess::NotRunning ) {
#ifdef Q_OS_LINUX
        externalPlayer->start( settings->GetExternalPlayer() + " \"" + filmsList->GetCurrentFilmFileName() +"\"" );
#elif defined(Q_OS_WIN32)
        externalPlayer->start( "\"" + settings->GetApplicationExternalPlayer() + "\" \"" + filmsList->GetCurrentFilmFileName() +"\"" );
#endif
    } else {
        externalPlayer->close();
    }
}

void MainWindow::PlayerStarted()
{
    dynamic_cast<QWidget*>( filmsView )->setEnabled( false );
    eFilter->setEnabled( false );
    bPlay->setText( tr( "STOP" ) );
}

void MainWindow::PlayerClosed()
{
    dynamic_cast<QWidget*>( filmsView )->setEnabled( true );
    eFilter->setEnabled( true );
    bPlay->setText( tr( "PLAY" ) );

    filmsList->IncCurrentFilmViewsCounter();

    if( bViewed->isEnabled() && !bViewed->isChecked() )
    {
        bViewed->setChecked( true );
        bViewed->clicked( true );
    }

}

void MainWindow::EditFilm()
{
    editFilmWindow->show( filmsList->GetCurrentFilm() );
}

void MainWindow::RemoveFilm()
{
    int res = QMessageBox::question( this,
                                     tr( "Remove film" ),
                                     tr( "Are you sure to remove \"%1\"?" ).arg( filmsList->GetCurrentFilmTitle() ),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

    if( res == QMessageBox::Yes )
    {
        filmsList->RemoveCurrentFilm();
    }
}

void MainWindow::FilmsFilter( QString key )
{
    statusbar->ShowLoading();

    QList<Film>* films = filmsList->GetFilmsFilteredBy( key );

    bool highlightUnavailable = settings->GetCheckFilesOnStartup();
    QColor unavailableColor = settings->GetUnavailableFileColor();

    filmsView->Clear();

    for( int i = 0; i < films->size(); i++ )
    {
        const Film& film = films->at(i);

        if( highlightUnavailable && !QFileInfo( film.GetFileName() ).exists() )
        {
            filmsView->AddItem( film, unavailableColor );
        }
        else
        {
            filmsView->AddItem( film );
        }
    }

    if( films->size() == filmsList->GetNumberOfFilms() )
    {
        statusbar->ShowTotal( filmsList->GetFilmsCount(), filmsList->GetIsViewedCount(), filmsList->GetIsFavouriteCount() );
    }
    else
    {
        statusbar->ShowFounded( films->size() );
    }

    filmsView->SetCurrentItemIndex( 0 );
    delete films;
}

void MainWindow::LoadSettings()
{
    QApplication::setStyle( settings->GetApplicationStyleName() );

    // Main window
    restoreGeometry( settings->GetMainWindowGeometry() );
    restoreState( settings->GetMainWindowState() );
    mainSplitter->restoreState( settings->GetMainWindowSplitterState() );
    actionShowToolbar->setChecked( toolbar->isVisibleTo( this ) );

    // Widgets
    filmsView->LoadSettings( settings );
    toolbar->LoadSettings( settings );
}

void MainWindow::SaveSettings()
{
    // Main window
    settings->SetMainWindowState( saveState() );
    settings->SetMainWindowGeometry( saveGeometry() );
    settings->SetMainWindowSplitterState( mainSplitter->saveState() );

    // Widgets
    filmsView->SaveSettings( settings );

    // Choosen film
    settings->SetCurrentFilmTitle( filmsList->GetCurrentFilmTitle() );

    settings->sync();
}

void MainWindow::SetupFilmsView()
{
    if( filmsView != nullptr )
    {
        delete filmsView;
    }

    QWidget* view = nullptr;

    switch( settings->GetFilmsViewMode() )
    {
        case Alexandra::GridMode :
        {
            view = new FilmsViewGrid( this );
            break;
        }

        // List view by default
        default :
        {
            view = new FilmsViewList( this );
        }
    }

    connect( view, SIGNAL( ItemClicked(QString) ), filmsList, SLOT( SetCurrentFilm(QString) ) );
    connect( view, SIGNAL( ItemClicked(QString) ), this, SLOT( ShowFilmInformation() ) );
    connect( view, SIGNAL( ItemDoubleClicked(QString) ), this, SLOT( PlayFilm() ) );

    filmsView = dynamic_cast<AbstractFilmsView*>( view );
    vlLeft->insertWidget( 0, view );
}

void MainWindow::SetupWindows()
{
    // Splashscreen
    splashScreen = new SplashScreen();
    splashScreen->show();
    connect( this, SIGNAL( Shown() ), splashScreen, SLOT( Close() ) );

    // Main window
    connect( toolbar, SIGNAL( actionExit() ), this, SLOT( close() ) );

    connect( filmsList, SIGNAL( DatabaseChanged() ), this, SLOT( SaveDatabase() ) );
    connect( filmsList, SIGNAL( DatabaseChanged() ), this, SLOT( ShowFilms() ) );
    connect( filmsList, SIGNAL( DatabaseReadError() ), this, SLOT( DatabaseReadError() ) );
    connect( filmsList, SIGNAL( DatabaseIsReadonly() ), this, SLOT( DatabaseIsReadonly() ) );
    connect( filmsList, SIGNAL( DatabaseIsEmpty() ), this, SLOT( DatabaseIsEmpty() ) );

    connect( bPlay, SIGNAL( clicked() ), this, SLOT( PlayFilm() ) );
    connect( bViewed, SIGNAL( clicked(bool) ), filmsList, SLOT( SetCurrentFilmIsViewed(bool) ) );
    connect( bFavourite, SIGNAL( clicked(bool) ), filmsList, SLOT( SetCurrentFilmIsFavourite(bool) ) );

    connect( eFilter, SIGNAL( textChanged(QString) ), this, SLOT( FilmsFilter(QString) ) );

    connect( externalPlayer, SIGNAL( started() ), this, SLOT( PlayerStarted() ) );
    connect( externalPlayer, SIGNAL( finished(int) ), this, SLOT( PlayerClosed() ) );

    // About window
    aboutWindow = new AboutWindow( this );

    connect( actionAbout, SIGNAL( triggered() ), aboutWindow, SLOT( show() ) );
    connect( actionAboutQt, SIGNAL( triggered() ), aboutWindow, SLOT( AboutQt() ) );

    // Add film window
    addFilmWindow = new AddFilmWindow( settings, this );

    connect( actionAdd, SIGNAL( triggered() ), addFilmWindow, SLOT( show() ) );
    connect( toolbar, SIGNAL( actionAdd() ), addFilmWindow, SLOT( show() ) );

    connect( addFilmWindow, SIGNAL( Done(Film) ), filmsList, SLOT( AddFilm(Film) ) );
    connect( addFilmWindow, SIGNAL( Done(Film) ), dynamic_cast<QWidget*>( filmsView ), SLOT( SelectItem(Film) ) );

    // Edit film window
    editFilmWindow = new EditFilmWindow( settings, this );

    connect( actionEdit, SIGNAL( triggered() ), this, SLOT( EditFilm() ) );
    connect( toolbar, SIGNAL( actionEdit() ), this, SLOT( EditFilm() ) );

    connect( editFilmWindow, SIGNAL( Done(Film) ), filmsList, SLOT( ChangeCurrentFilm(Film) ) );

    // Remove film dialog
    connect( actionRemove, SIGNAL( triggered() ), this, SLOT( RemoveFilm() ) );
    connect( toolbar, SIGNAL( actionRemove() ), this, SLOT( RemoveFilm() ) );

    // Film info window
    filmInfoWindow = new FilmInfoWindow( this );

    connect( bTechInformation, SIGNAL( clicked() ), filmInfoWindow, SLOT( show() ) );

    connect( filmInfoWindow, SIGNAL( ShortInfoLoaded(QString) ), this, SLOT( ShowShortTechnicalInfo(QString) ) );

    // Search window
    searchWindow = new SearchWindow( filmsList->GetFilmsList(), this );

    connect( actionSearch, SIGNAL( triggered() ), searchWindow, SLOT( show() ) );
    connect( toolbar, SIGNAL( actionSearch() ), searchWindow, SLOT( show() ) );

    connect( searchWindow, SIGNAL( FilmSelected(QString) ), filmsList, SLOT( SetCurrentFilm(QString) ) );
    connect( searchWindow, SIGNAL( FilmSelected(QString) ), dynamic_cast<QWidget*>( filmsView ), SLOT( SelectItem(QString) ) );

    // Settings window
    settingsWindow = new SettingsWindow( settings, this );

    connect( actionSettings, SIGNAL( triggered() ), settingsWindow, SLOT( show() ) );

    connect( settingsWindow, SIGNAL( SettingsChanged() ), this, SLOT( ReloadSettings() ) );
    connect( settingsWindow, SIGNAL( DatabaseSettingsChanged() ), this, SLOT( ReloadDatabase() ) );
    ///connect( settingsWindow, SIGNAL( EraseDatabase() ), tableFilms, SLOT( EraseDatabase() ) );

    // Random film
    connect( actionRandom, SIGNAL( triggered() ), dynamic_cast<QWidget*>( filmsView ), SLOT( SelectRandomItem() ) );
    connect( toolbar, SIGNAL( actionRandom() ), dynamic_cast<QWidget*>( filmsView ), SLOT( SelectRandomItem() ) );
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
