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
#include <QCompleter>
#include <QFileInfo>
#include <QLineEdit>
#include <QMessageBox>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
    // Interface
    setupUi( this );
    setWindowTitle( Alexandra::appNameGui );

    // Data
    contextMenu = new FilmsViewContextMenu( this );
    externalPlayer = new QProcess( this );
    settings = new AlexandraSettings( this );
    filmsList = new FilmsList( settings, this );

    SetupWindows();
    SetupFilmsView();
    LoadSettings();

    filmsList->LoadFromFile( settings->GetDatabaseFilePath() );
    filmsList->SetCurrentFilm( settings->GetCurrentFilmTitle() );
    filmsView->SelectItem( settings->GetCurrentFilmTitle() );
}

MainWindow::~MainWindow()
{
    // Child windows
    delete aboutWindow;
    delete editFilmWindow;
    delete filmInfoWindow;
    delete filmScannerWindow;
    delete movedFilmsWindow;
    delete searchWindow;
    delete settingsWindow;
    delete splashScreen;

    // Variables
    delete settings;
    delete externalPlayer;
    delete filmsList;
    delete filmsView;
    delete contextMenu;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    hide();
    SaveSettings();
    event->accept();
}

void MainWindow::ShowFullScreen( bool isFullScreen )
{
    if( isFullScreen )
    {
        showFullScreen();
    }
    else
    {
        showNormal();
    }
}

void MainWindow::SaveDatabase()
{
    filmsList->SaveToFileAsync( settings->GetDatabaseFilePath() );
}

void MainWindow::EraseDatabase()
{
    if( filmsList->GetFilmsCount() == 0 )
    {
        QMessageBox::information( this, tr( "Erase database" ), tr( "Nothing to erase." ) );
    }
    else
    {
        filmsList->EraseAll();
        QMessageBox::information( this, tr( "Erase database" ), tr( "Done!" ) );
    }
}

void MainWindow::ResetStatistics()
{
    filmsList->ResetViews();
    QMessageBox::information( this, tr( "Reset statistics" ), tr( "Done!" ) );
}

void MainWindow::ReloadDatabase()
{
    filmsList->LoadFromFile( settings->GetDatabaseFilePath() );
    filmsView->SelectItem( settings->GetCurrentFilmTitle() );
}

void MainWindow::ReloadSettings()
{
    QString style = settings->GetApplicationStyleName();

    if( style == tr( "Theme" ) )
    {
        QFile f( Alexandra::themes[ settings->GetApplicationThemeIndex() ].path + "style.qss" );
        f.open( QFile::ReadOnly );
        qApp->setStyleSheet( QString::fromUtf8( f.readAll() ) );
        f.close();
    }
    else
    {
        qApp->setStyleSheet( "" );
        qApp->setStyle( style );
    }

    wRight->setVisible( settings->GetMainWindowShowRightPanel() );
    toolbar->LoadSettings( settings );
    filmsView->ReloadSettings( settings );
}

void MainWindow::ReloadView()
{
    SetupFilmsView();
    filmsView->LoadSettings( settings );
    ShowFilms();
}

void MainWindow::DatabaseIsLoaded()
{
    SetAllFunctionsEnabled( true );
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

    StatusbarShowTotal();
    filmsView->SetCurrentItemIndex( currentIndex );
    emit Shown();
}

void MainWindow::ShowFilmInformation()
{
    const Film* f = filmsList->GetCurrentFilm();

    if( f == nullptr )
    {
        return;
    }

    // Buttons and technical information
    bTechInformation->setEnabled( false );

    if( QFileInfo( f->GetFileName() ).exists() )
    {
        // File exists
        filmInfoWindow->LoadTechnicalInfoAsync( f->GetFileName() );
        bPlay->setEnabled( true );
    }
    else
    {
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

    if( p.isNull() )
    {
        p.load( ":/standart-poster" );
    }

    p = p.scaledToWidth( lPosterImage->maximumWidth(), Qt::SmoothTransformation );
    lPosterImage->setPixmap( p );
}

void MainWindow::ShowFilmContextMenu( QPoint p )
{
    contextMenu->SetState( filmsList->GetCurrentFilm() );
    contextMenu->exec( dynamic_cast<QWidget*>( filmsView )->mapToGlobal( p ) );
}

void MainWindow::ShowShortTechnicalInfo( QString info )
{
    lTechInformation->setText( info );
    bTechInformation->setEnabled( true );
}

void MainWindow::PlayFilm()
{
    if( !bPlay->isEnabled() ) return;

    if( externalPlayer->state() == QProcess::NotRunning )
    {
#ifdef Q_OS_LINUX
        externalPlayer->start( settings->GetExternalPlayer() + " \"" + filmsList->GetCurrentFilmFileName() +"\"" );
#elif defined(Q_OS_WIN32)
        externalPlayer->start( "\"" + settings->GetExternalPlayer() + "\" \"" + filmsList->GetCurrentFilmFileName() +"\"" );
#endif
    }
    else
    {
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
        filmsView->RemoveCurrentItem();
    }
}

void MainWindow::RemoveFile()
{
    int res = QMessageBox::question( this,
                                     tr( "Remove file" ),
                                     tr( "Are you sure to remove file \"%1\"?" ).arg( filmsList->GetCurrentFilmFileName() ),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

    if( res == QMessageBox::Yes )
    {
        if( QFile( filmsList->GetCurrentFilmFileName() ).remove() == true )
        {
            filmsList->RemoveCurrentFilm();
        }
        else
        {
            QMessageBox::warning( this,
                                  tr( "Remove file" ),
                                  tr( "Unable to remove file \"%1\"!" ).arg( filmsList->GetCurrentFilmFileName() ) );
        }
    }
}

void MainWindow::FilmScanner()
{
    filmScannerWindow->show( filmsList->GetFilmsFileNames() );
}

void MainWindow::MovedFilms()
{
    movedFilmsWindow->show( filmsList->GetUnavailableFilms() );
}

void MainWindow::Statistics()
{
    statisticsWindow->show( filmsList->GetFilmsList() );
}

void MainWindow::SetupCompleter()
{
    delete eFilter->completer();

    const QList<Film>* films = filmsList->GetFilmsList();
    QStringList words;

    for( int i = 0; i < films->size(); i++ )
    {
        words.append( films->at(i).GetTitle() );
    }

    QCompleter* c = new QCompleter( words, this );
    c->setCaseSensitivity( Qt::CaseInsensitive );
    eFilter->setCompleter( c );
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
        StatusbarShowTotal();
    }
    else
    {
        statusbar->ShowFounded( films->size() );
    }

    filmsView->SetCurrentItemIndex( 0 );
    delete films;
}

void MainWindow::UpdateCurrentFilm()
{
    if( filmsList->GetCurrentFilm() != nullptr )
    {
        filmsView->SetCurrentItemTo( *filmsList->GetCurrentFilm() );
    }
}

void MainWindow::StatusbarShowTotal()
{
    statusbar->ShowTotal( filmsList->GetFilmsCount(), filmsList->GetIsViewedCount(), filmsList->GetIsFavouriteCount() );
}

void MainWindow::LoadSettings()
{
    QString style = settings->GetApplicationStyleName();

    if( style == tr( "Theme" ) )
    {
        QFile f( Alexandra::themes[ settings->GetApplicationThemeIndex() ].path + "style.qss" );
        f.open( QFile::ReadOnly );
        qApp->setStyleSheet( QString::fromUtf8( f.readAll() ) );
        f.close();
    }
    else
    {
        qApp->setStyle( style );
    }

    // Main window
    restoreGeometry( settings->GetMainWindowGeometry() );
    restoreState( settings->GetMainWindowState() );
    mainSplitter->restoreState( settings->GetMainWindowSplitterState() );
    ShowFullScreen( settings->GetMainWindowIsFullscreen() );
    actionShowFullscreen->setChecked( isFullScreen() );
    actionShowToolbar->setChecked( toolbar->isVisibleTo( this ) );
    wRight->setVisible( settings->GetMainWindowShowRightPanel() );

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
    settings->SetMainWindowIsFullscreen( isFullScreen() );

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

    // Base signals
    connect( view, SIGNAL( ItemClicked(QString) ), filmsList, SLOT( SetCurrentFilm(QString) ) );
    connect( view, SIGNAL( ItemClicked(QString) ), this, SLOT( ShowFilmInformation() ) );
    connect( view, SIGNAL( ItemDoubleClicked(QString) ), this, SLOT( PlayFilm() ) );
    connect( view, SIGNAL( ContextMenuRequested(QPoint) ), this, SLOT( ShowFilmContextMenu(QPoint) ) );
    // Add film window
    connect( addFilmWindow, SIGNAL( Done(Film) ), view, SLOT( AddItem(Film) ) );
    connect( addFilmWindow, SIGNAL( Done(Film) ), view, SLOT( SelectItem(Film) ) );
    // Edit film window
    connect( editFilmWindow, SIGNAL( Done(Film) ), view, SLOT( SetCurrentItemTo(Film) ) );
    // Search window
    connect( searchWindow, SIGNAL( FilmSelected(QString) ), view, SLOT( SelectItem(QString) ) );
    // Random film function
    connect( actionRandom, SIGNAL( triggered() ), view, SLOT( SelectRandomItem() ) );
    connect( toolbar, SIGNAL( actionRandom() ), view, SLOT( SelectRandomItem() ) );

    filmsView = dynamic_cast<AbstractFilmsView*>( view );
    vlLeft->insertWidget( 0, view );
    view->setFocus();
}

void MainWindow::SetupWindows()
{
    /// Splashscreen
    splashScreen = new SplashScreen();
    splashScreen->show();

    connect( this, &MainWindow::Shown, splashScreen, &SplashScreen::Close );

    /// Main window
    connect( actionShowFullscreen, &QAction::toggled, this, &MainWindow::ShowFullScreen );
    connect( toolbar, &ToolBar::actionExit, this, &MainWindow::close );

    connect( filmsList, &FilmsList::DatabaseLoaded, this, &MainWindow::ShowFilms );
    connect( filmsList, &FilmsList::DatabaseLoaded, this, &MainWindow::DatabaseIsLoaded );

    connect( filmsList, &FilmsList::DatabaseChanged, this, &MainWindow::SaveDatabase );
    connect( filmsList, &FilmsList::DatabaseChanged, this, &MainWindow::UpdateCurrentFilm );
    connect( filmsList, &FilmsList::DatabaseChanged, this, &MainWindow::StatusbarShowTotal );

    connect( filmsList, &FilmsList::DatabaseReadError, this, &MainWindow::DatabaseReadError );
    connect( filmsList, &FilmsList::DatabaseIsReadonly, this, &MainWindow::DatabaseIsReadonly );
    connect( filmsList, &FilmsList::DatabaseIsEmpty, this, &MainWindow::DatabaseIsEmpty );

    // Play button
    connect( bPlay, &QPushButton::clicked, this, &MainWindow::PlayFilm );
    connect( contextMenu, &FilmsViewContextMenu::actionPlay, this, &MainWindow::PlayFilm );
    // Viewed button
    connect( bViewed, &QPushButton::clicked, filmsList, &FilmsList::SetCurrentFilmIsViewed );
    connect( contextMenu, &FilmsViewContextMenu::actionIsViewed, filmsList, &FilmsList::SetCurrentFilmIsViewed );
    connect( contextMenu, &FilmsViewContextMenu::actionIsViewed, this, &MainWindow::ShowFilmInformation );
    // Favourite button
    connect( bFavourite, &QPushButton::clicked, filmsList, &FilmsList::SetCurrentFilmIsFavourite );
    connect( contextMenu, &FilmsViewContextMenu::actionIsFavourite, filmsList, &FilmsList::SetCurrentFilmIsFavourite );
    connect( contextMenu, &FilmsViewContextMenu::actionIsFavourite, this, &MainWindow::ShowFilmInformation );

    connect( eFilter, &QLineEdit::textChanged, this, &MainWindow::FilmsFilter );
    connect( filmsList, &FilmsList::DatabaseLoaded, this, &MainWindow::SetupCompleter );
    connect( filmsList, &FilmsList::DatabaseChanged, this, &MainWindow::SetupCompleter );

    connect( externalPlayer, &QProcess::started, this, &MainWindow::PlayerStarted );
    connect( externalPlayer, SIGNAL( finished(int) ), this, SLOT( PlayerClosed() ) );

    /// About window
    aboutWindow = new AboutWindow( this );

    connect( actionAbout, &QAction::triggered, aboutWindow, &AboutWindow::show );
    connect( actionAboutQt, &QAction::triggered, aboutWindow, &AboutWindow::AboutQt );

    /// Add film window
    addFilmWindow = new AddFilmWindow( settings, this );

    connect( actionAdd, &QAction::triggered, addFilmWindow, &AddFilmWindow::show );
    connect( toolbar, &ToolBar::actionAdd, addFilmWindow, &AddFilmWindow::show );

    connect( addFilmWindow, &AddFilmWindow::Done, filmsList, &FilmsList::AddFilm );
    connect( addFilmWindow, &AddFilmWindow::Done, this, &MainWindow::DatabaseIsLoaded );

    /// Edit film window
    editFilmWindow = new EditFilmWindow( settings, this );

    connect( actionEdit, &QAction::triggered, this, &MainWindow::EditFilm );
    connect( toolbar, &ToolBar::actionEdit, this, &MainWindow::EditFilm );
    connect( contextMenu, &FilmsViewContextMenu::actionEdit, this, &MainWindow::EditFilm );

    connect( editFilmWindow, &EditFilmWindow::Done, filmsList, &FilmsList::ChangeCurrentFilm );
    connect( editFilmWindow, &EditFilmWindow::Done, this, &MainWindow::ShowFilmInformation );

    /// Remove film dialog
    connect( actionRemove, &QAction::triggered, this, &MainWindow::RemoveFilm );
    connect( toolbar, &ToolBar::actionRemove, this, &MainWindow::RemoveFilm );
    connect( contextMenu, &FilmsViewContextMenu::actionRemove, this, &MainWindow::RemoveFilm );

    /// Remove file
    connect( contextMenu, &FilmsViewContextMenu::actionRemoveFile, this, &MainWindow::RemoveFile );

    /// Film info window
    filmInfoWindow = new FilmInfoWindow( this );

    connect( bTechInformation, &QPushButton::clicked, filmInfoWindow, &FilmInfoWindow::show );
    connect( contextMenu, &FilmsViewContextMenu::actionShowInfo, filmInfoWindow, &FilmInfoWindow::show );

    connect( filmInfoWindow, &FilmInfoWindow::ShortInfoLoaded, this, &MainWindow::ShowShortTechnicalInfo );

    /// Search window
    searchWindow = new SearchWindow( filmsList->GetFilmsList(), this );

    connect( actionSearch, &QAction::triggered, searchWindow, &SearchWindow::show );
    connect( toolbar, &ToolBar::actionSearch, searchWindow, &SearchWindow::show );

    connect( searchWindow, &SearchWindow::FilmSelected, filmsList, &FilmsList::SetCurrentFilm );

    /// Settings window
    settingsWindow = new SettingsWindow( settings, this );

    connect( actionSettings, &QAction::triggered, settingsWindow, &SettingsWindow::show );

    connect( settingsWindow, &SettingsWindow::SettingsChanged, this, &MainWindow::ReloadSettings );
    connect( settingsWindow, &SettingsWindow::ViewChanged, this, &MainWindow::ReloadView );
    connect( settingsWindow, &SettingsWindow::DatabaseSettingsChanged, this, &MainWindow::ReloadDatabase );
    connect( settingsWindow, &SettingsWindow::EraseDatabase, this, &MainWindow::EraseDatabase );

    /// Film scanner window
    filmScannerWindow = new FilmScannerWindow( settings, this );

    connect( actionFilmScanner, &QAction::triggered, this, &MainWindow::FilmScanner );
    connect( toolbar, &ToolBar::actionFilmScanner, this, &MainWindow::FilmScanner );

    connect( filmScannerWindow, &FilmScannerWindow::AddFilms, filmsList, &FilmsList::AddFilms );
    connect( filmScannerWindow, &FilmScannerWindow::AddFilms, this, &MainWindow::ShowFilms );

    /// Moved films window
    movedFilmsWindow = new MovedFilmsWindow( settings, this );

    connect( actionMovedFilms, &QAction::triggered, this, &MainWindow::MovedFilms );

    connect( movedFilmsWindow, &MovedFilmsWindow::FilmsMoved, this, &MainWindow::ShowFilms );
    connect( movedFilmsWindow, &MovedFilmsWindow::FilmsMoved, filmsList, &FilmsList::FilmsMoved );

    /// Statistic window
    statisticsWindow = new StatisticsWindow( this );

    connect( actionStatistics, &QAction::triggered, this, &MainWindow::Statistics );

    connect( statisticsWindow, &StatisticsWindow::ResetStatistics, this, &MainWindow::ResetStatistics );
}

void MainWindow::ClearTextFields()
{
    lPosterImage->setPixmap( QPixmap( ":/standart-poster" ).scaledToWidth( lPosterImage->maximumWidth(),
                                                                           Qt::SmoothTransformation ) );
    lFilmTitle->clear();
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
