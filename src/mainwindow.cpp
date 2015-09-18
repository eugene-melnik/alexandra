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

#include "filesextensions.h"
#include "filmsviewgrid.h"
#include "filmsviewlist.h"
#include "mainwindow.h"
#include "playlist.h"
#include "version.h"

#include <list>
#include <QColor>
#include <QCompleter>
#include <QFileInfo>
#include <QMessageBox>

MainWindow::MainWindow( AlexandraSettings* s ) : QMainWindow(), settings( s )
{
    setupUi( this );
    setWindowTitle( Alexandra::appNameGui );

    SetupWindows();
    SetupFilmsView();
    LoadSettings();

    filmsList->LoadFromFile( settings->GetDatabaseFilePath() );
    filmsList->SetCurrentFilm( settings->GetCurrentFilmTitle() );
}

void MainWindow::AddFilmsFromOutside( const QStringList& films )
{
    QList<Film> newFilms;
    QString messageText;

    for( const QString& film : films )
    {
        QFileInfo filmInfo( film );
        QString fileSuffix = "*." + filmInfo.suffix();

        if( FilesExtensions().GetFilmExtensionsForDirFilter().contains( fileSuffix ) )
        {
            Film f;
            f.SetFileName( film );
            f.SetTitle( filmInfo.completeBaseName() );
            newFilms.append( f );
            messageText.append( QString::number( newFilms.size() ) + ") " + filmInfo.fileName() + "\n" );
        }
    }

    if( newFilms.size() == 0 )
    {
        QMessageBox::information( this,
                                  tr( "Add films" ),
                                  tr( "There is nothing to add." ) );
        return;
    }

    messageText = tr( "Add the following film(s)?\n" ) + messageText;

    int res = QMessageBox::question( this,
                                     tr( "Add films" ),
                                     messageText,
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::Yes );

    if( res == QMessageBox::Yes )
    {
        filmsList->AddFilms( &newFilms );
        DatabaseIsLoaded();
        ShowFilms();
        QMessageBox::information( this, tr( "Add films" ), tr( "Done!" ) );
    }
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    hide();
    SaveSettings();
    event->accept();
}

void MainWindow::QuickSearchEscBehavior()
{
    // Pressing key 'Esc' sets focus to the films list,
    // pressing again clears the quick search line
    QWidget* view = dynamic_cast<QWidget*>( filmsView );

    if( view == focusWidget() )
    {
        eFilter->clear();
    }

    view->setFocus();
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
    LoadAppearance();
    LoadShorcuts();
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
    ClearTextFields();
    SetEmptyMode();
    lFilmTitle->setText( tr( "Database is empty!" ) );

    repaint(); // Need for removing the artifacts

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
    SetProgressMaximum( filmsList->GetFilmsCount() );
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

        SetProgressValue( i + 1 );
    }

    if( currentIndex > 0 )
    {
        filmsView->SetCurrentItemIndex( currentIndex );
    }
    else
    {
        filmsView->SelectItem( settings->GetCurrentFilmTitle() );
    }

    StatusbarShowTotal();
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
#ifdef MEDIAINFO_SUPPORT
        filmInfoWindow->LoadTechnicalInfoAsync( f->GetFileName() );
#endif
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
    lOriginalTitle->setVisible( !f->GetOriginalTitle().isEmpty() );

    lTagline->setText( tr( "<b>Tagline:</b> %1" ).arg( f->GetTagline() ) );
    lTagline->setVisible( !f->GetTagline().isEmpty() );

    lGenre->setText( tr( "<b>Genre:</b> %1" ).arg( f->GetGenre() ) );
    lGenre->setVisible( !f->GetGenre().isEmpty() );

    lYear->setText( tr( "<b>Year:</b> %1" ).arg( f->GetYear() ) );
    lYear->setVisible( f->GetYear() != 0 );

    lBudget->setText( tr( "<b>Budget:</b> %1" ).arg( f->GetBudgetStr() ) );
    lBudget->setVisible( !f->GetBudgetStr().isEmpty() );

    lCountry->setText( tr( "<b>Country:</b> %1" ).arg( f->GetCountry() ) );
    lCountry->setVisible( !f->GetCountry().isEmpty() );

    lScreenwriter->setText( tr( "<b>Screenwriter:</b> %1" ).arg( f->GetScreenwriter() ) );
    lScreenwriter->setVisible( !f->GetScreenwriter().isEmpty() );

    lDirector->setText( tr( "<b>Director:</b> %1" ).arg( f->GetDirector() ) );
    lDirector->setVisible( !f->GetDirector().isEmpty() );

    lProducer->setText( tr( "<b>Producer:</b> %1" ).arg( f->GetProducer() ) );
    lProducer->setVisible( !f->GetProducer().isEmpty() );

    lComposer->setText( tr( "<b>Composer:</b> %1" ).arg( f->GetComposer() ) );
    lComposer->setVisible( !f->GetComposer().isEmpty() );

    lStarring->setText( tr( "<b>Starring:</b> %1" ).arg( f->GetStarring() ) );
    lStarring->setVisible( !f->GetStarring().isEmpty() );

    lRating->setText( tr( "<b>Rating:</b> %1" ).arg( f->GetRatingStr() ) );
    lRating->setVisible( f->GetRating() != 1 );

    lDescription->setText( tr( "<b>Description:</b> %1" ).arg( f->GetDescription() ) );
    lDescription->setVisible( !f->GetDescription().isEmpty() );

    lTags->setText( tr( "<b>Tags:</b> %1" ).arg( f->GetTags() ) );
    lTags->setVisible( !f->GetTags().isEmpty() );

    lTechInformation->setVisible( false );

    // Buttons
    bViewed->setChecked( f->GetIsViewed() );
    bFavourite->setChecked( f->GetIsFavourite() );

    // Poster
    QPixmap p;

    if( f->GetIsPosterExists() )
    {
        p.load( settings->GetPostersDirPath() + "/" + f->GetPosterName() );
    }

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

void MainWindow::ShowShortTechnicalInfo( const QString& info )
{
    lTechInformation->setText( info );
    bTechInformation->setEnabled( true );
    lTechInformation->setVisible( true );
}

void MainWindow::AddToPlaylist() //TODO:
{
    bPlay->setText( tr( "Play list" ) );
    wPlaylist->show();

    lwPlaylist->AddItem( filmsList->GetCurrentFilmTitle(),
                         filmsList->GetCurrentFilmFileName() );
}

void MainWindow::PlaylistCleared()
{
    bPlay->setText( tr( "&PLAY" ) );
    wPlaylist->hide();
}

void MainWindow::DoubleClickBehavior()
{
    QString s = settings->GetPlayerDoubleClickBehavior();

    if( s == "play" )
    {
        PlayFilm();
    }
    else if( s == "add-to-list" )
    {
        AddToPlaylist();
    }
    else
    // "auto" mode -- plays film if playlist is empty
    // and adds film to playlist otherwise
    {
        if( lwPlaylist->IsEmpty() )
        {
            PlayFilm();
        }
        else
        {
            AddToPlaylist();
        }
    }
}

void MainWindow::PlayFilm() // TODO:
{
    if( !bPlay->isEnabled() ) return;

    if( externalPlayer->state() == QProcess::NotRunning )
    {
        QString fileToPlay;

        if( lwPlaylist->IsEmpty() )
        {
            fileToPlay = filmsList->GetCurrentFilmFileName();
        }
        else
        {
            fileToPlay = PlayList( lwPlaylist->GetPathes() ).CreateTempListM3U8();
        }

#ifdef Q_OS_LINUX
        externalPlayer->start( settings->GetExternalPlayer() + " \"" + fileToPlay +"\"" );
#elif defined(Q_OS_WIN32)
        externalPlayer->start( "\"" + settings->GetExternalPlayer() + "\" \"" + fileToPlay +"\"" );
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
    bPlay->setText( tr( "&PLAY" ) );

    // TODO: make all films of playlist is viewed
    // need to think about this
    if( lwPlaylist->IsEmpty() )
    {
        filmsList->IncCurrentFilmViewsCounter();

        if( bViewed->isEnabled() && !bViewed->isChecked() )
        {
            bViewed->setChecked( true );
            UpdateCurrentFilm();
        }
    }
    else
    {
        lwPlaylist->Clear();
    }

    dynamic_cast<QWidget*>( filmsView )->setFocus();
}

void MainWindow::RemoveFilm()
{
    QStringList films = filmsView->GetSelectedItemsList();
    int res;

    if( films.count() == 1 ) // One film selected
    {
        res = QMessageBox::question( this,
                                     tr( "Remove film" ),
                                     tr( "Are you sure to remove \"%1\"?" ).arg( filmsList->GetCurrentFilmTitle() ),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No );
    }
    else // More than one film selected
    {
        QString message = tr( "Are you sure to remove following films?\n" );
        const int maxFilmsCount = 20;
        int counter = 1;

        for( int i = 0; i < std::min( films.size(), maxFilmsCount ); i++ )
        {
            message.append( QString( "%1) %2\n" ).arg( counter++ ).arg( films[i] ) );
        }

        if( films.size() > maxFilmsCount )
        {
            message.append( "..." );
        }

        res = QMessageBox::question( this,
                                     tr( "Remove film" ),
                                     message,
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No );
    }

    if( res == QMessageBox::Yes )
    {
        for( const QString& s : films )
        {
            filmsView->RemoveItemByTitle( s );
            filmsList->RemoveFilmByTitle( s );
        }

        filmsView->SetCurrentItemIndex( 0 );
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
            filmsView->RemoveCurrentItem();
        }
        else
        {
            QMessageBox::warning( this,
                                  tr( "Remove file" ),
                                  tr( "Unable to remove file \"%1\"!" ).arg( filmsList->GetCurrentFilmFileName() ) );
        }
    }
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

void MainWindow::FilmsFilter( const QString& key, SearchEdit::FilterBy filters )
{
    if( key.isEmpty() )
    {
        QString foundedTitle = filmsList->GetCurrentFilmTitle();
        ShowFilms();
        filmsView->SelectItem( foundedTitle );
        return;
    }

    // Filtering
    statusbar->ShowLoading();

    const QList<Film>* films = filmsList->GetFilmsList();
    std::list<Film> filteredFilms;

    for( const Film& film : *films ) // TODO: Move algorithm to the separate Search engine class
    {
        if( ( filters & SearchEdit::Title ) && film.GetTitle().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film );

        if( ( filters & SearchEdit::Tags ) && film.GetTags().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film );

        if( ( filters & SearchEdit::Genre ) && film.GetGenre().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film );

        if( ( filters & SearchEdit::Starring ) && film.GetStarring().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film );

        if( ( filters & SearchEdit::Director ) && film.GetDirector().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film );

        if( ( filters & SearchEdit::Producer ) && film.GetProducer().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film );

        if( ( filters & SearchEdit::Screenwriter ) && film.GetScreenwriter().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film );

        if( ( filters & SearchEdit::Composer ) && film.GetComposer().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film );

        if( ( filters & SearchEdit::Country ) && film.GetCountry().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film );

        if( ( filters & SearchEdit::Description ) && film.GetDescription().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film );
    }

    filteredFilms.sort();
    filteredFilms.unique();

    // Show founded
    bool highlightUnavailable = settings->GetCheckFilesOnStartup();
    QColor unavailableColor = settings->GetUnavailableFileColor();

    filmsView->Clear();

    for( Film& film : filteredFilms )
    {
        if( highlightUnavailable && !QFileInfo( film.GetFileName() ).exists() )
        {
            filmsView->AddItem( film, unavailableColor );
        }
        else
        {
            filmsView->AddItem( film );
        }
    }

    if( filteredFilms.empty() )
    {
        ClearTextFields();
        statusbar->ShowFounded( 0 );
        lFilmTitle->setText( tr( "Nothing was found! Try to change keyword or search settings." ) );
    }
    else
    {
        statusbar->ShowFounded( filteredFilms.size() );
    }

    filmsView->SetCurrentItemIndex( 0 );
}

void MainWindow::UpdateCurrentFilm()
{
    if( filmsList->GetCurrentFilm() != nullptr )
    {
        filmsView->SetCurrentItemTo( *filmsList->GetCurrentFilm() );
    }
}

void MainWindow::SetProgressMaximum( int n )
{
    if( splashScreen != nullptr) splashScreen->SetProgressMaximum( n );
    statusbar->SetProgresMaximum( n );
}

void MainWindow::SetProgressValue( int n )
{
    if( splashScreen != nullptr) splashScreen->SetProgressValue( n + 1 );
    statusbar->SetProgressValue( n + 1 );
}

void MainWindow::StatusbarShowTotal()
{
    statusbar->ShowTotal( filmsList->GetFilmsCount(),
                          filmsList->GetIsViewedCount(),
                          filmsList->GetIsFavouriteCount() );
}

void MainWindow::LoadSettings()
{
    LoadAppearance();
    LoadShorcuts();

    // Main window
    restoreGeometry( settings->GetMainWindowGeometry() );
    restoreState( settings->GetMainWindowState() );
    mainSplitter->restoreState( settings->GetMainWindowSplitterState() );
    actionShowFullscreen->setChecked( isFullScreen() );
    actionShowToolbar->setChecked( toolbar->isVisibleTo( this ) );
    wRight->setVisible( settings->GetMainWindowShowRightPanel() );

    // Widgets
    eFilter->LoadSettings( settings );
    filmsView->LoadSettings( settings );
    toolbar->LoadSettings( settings );
}

void MainWindow::LoadAppearance()
{
    // Font
    QFont font;
    font.fromString( settings->GetApplicationFont() );
    qApp->setFont( font );

    // Style
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
}

void MainWindow::LoadShorcuts()
{
    actionAdd->setShortcut( QKeySequence( settings->GetShortcutAddFilm() ) );
    actionEdit->setShortcut( QKeySequence( settings->GetShortcutEditFilm() ) );
    actionRemove->setShortcut( QKeySequence( settings->GetShortcutRemoveFilm() ) );

    actionRandom->setShortcut( QKeySequence( settings->GetShortcutSelectRandomFilm() ) );
    bActivateQuickSearch->setShortcut( QKeySequence( settings->GetShortcutActivateQuickSearch() ) );
    actionSearch->setShortcut( QKeySequence( settings->GetShortcutAdvancedSearch() ) );

    bPlay->setShortcut( QKeySequence( settings->GetShortcutPlay() ) );

    actionSettings->setShortcut( QKeySequence( settings->GetShortcutSettings() ) );
    actionShowToolbar->setShortcut( QKeySequence( settings->GetShortcutShowToolbar() ) );
    actionShowFullscreen->setShortcut( QKeySequence( settings->GetShortcutShowFullscreen() ) );

    actionExit->setShortcut( QKeySequence( settings->GetShortcutExit() ) );
}

void MainWindow::SaveSettings()
{
    // Main window
    settings->SetMainWindowState( saveState() );
    settings->SetMainWindowGeometry( saveGeometry() );
    settings->SetMainWindowSplitterState( mainSplitter->saveState() );

    // Widgets
    eFilter->SaveSettings( settings );
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
        settings->SetCurrentFilmTitle( filmsList->GetCurrentFilmTitle() );
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
    connect( view, SIGNAL( ItemDoubleClicked(QString) ), this, SLOT( DoubleClickBehavior() ) );
    connect( view, SIGNAL( ContextMenuRequested(QPoint) ), this, SLOT( ShowFilmContextMenu(QPoint) ) );
    // Add film window
    connect( addFilmWindow, SIGNAL( Done(Film) ), view, SLOT( AddItem(Film) ) );
    connect( addFilmWindow, SIGNAL( Done(Film) ), view, SLOT( SelectItem(Film) ) );
    // Search window
    connect( searchWindow, SIGNAL( FilmSelected(QString) ), view, SLOT( SelectItem(QString) ) );
    // Random film function
    connect( actionRandom, SIGNAL( triggered() ), view, SLOT( SelectRandomItem() ) );
    connect( toolbar, SIGNAL( actionRandom() ), view, SLOT( SelectRandomItem() ) );

    filmsView = dynamic_cast<AbstractFilmsView*>( view );
    vlLeft->insertWidget( 0, view );
    eFilter->clear();

    view->show();
    view->setFocus();
}

void MainWindow::SetupWindows()
{
    /// Splashscreen
    if( settings->GetApplicationShowSplashScreen() )
    {
        splashScreen = new SplashScreen();
        connect( this, &MainWindow::Shown, splashScreen, &SplashScreen::Close );
        splashScreen->show();

        qApp->processEvents(); // For splashscreen drawing
        qApp->processEvents(); // WTF: works only if run twice...
    }

    /// Main window
    contextMenu = new FilmsViewContextMenu( this );
    filmsList = new FilmsList( settings, this );

    connect( this, &MainWindow::Shown, this, &MainWindow::show );

    connect( actionShowFullscreen, &QAction::toggled, this, &MainWindow::ShowFullScreen );
    connect( toolbar, &ToolBar::actionExit, this, &MainWindow::close );

    connect( filmsList, &FilmsList::DatabaseLoaded, this, &MainWindow::ShowFilms );
    connect( filmsList, &FilmsList::DatabaseLoaded, this, &MainWindow::DatabaseIsLoaded );

    connect( filmsList, &FilmsList::DatabaseChanged, this, &MainWindow::SaveDatabase );
    connect( filmsList, &FilmsList::DatabaseChanged, this, &MainWindow::StatusbarShowTotal );

    connect( filmsList, &FilmsList::DatabaseReadError, this, &MainWindow::DatabaseReadError );
    connect( filmsList, &FilmsList::DatabaseIsReadonly, this, &MainWindow::DatabaseIsReadonly );
    connect( filmsList, &FilmsList::DatabaseIsEmpty, this, &MainWindow::DatabaseIsEmpty );

    // Player
    externalPlayer = new QProcess( this );

    // Playlist
    connect( bAddToPlaylist, &QPushButton::clicked, this, &MainWindow::AddToPlaylist );
    connect( contextMenu, &FilmsViewContextMenu::actionAddToList, this, &MainWindow::AddToPlaylist );

    connect( lwPlaylist, &PlayListWidget::Cleared, this, &MainWindow::PlaylistCleared );
    wPlaylist->hide();

    // Play button
    connect( bPlay, &QPushButton::clicked, this, &MainWindow::PlayFilm );
    connect( contextMenu, &FilmsViewContextMenu::actionPlay, this, &MainWindow::PlayFilm );
    // Viewed button
    connect( bViewed, &QPushButton::clicked, filmsList, &FilmsList::SetCurrentFilmIsViewed );
    connect( bViewed, &QPushButton::clicked, this, &MainWindow::UpdateCurrentFilm );
    connect( contextMenu, &FilmsViewContextMenu::actionIsViewed, filmsList, &FilmsList::SetCurrentFilmIsViewed );
    connect( contextMenu, &FilmsViewContextMenu::actionIsViewed, this, &MainWindow::UpdateCurrentFilm );
    connect( contextMenu, &FilmsViewContextMenu::actionIsViewed, bViewed, &QPushButton::setChecked );
    // Favourite button
    connect( bFavourite, &QPushButton::clicked, filmsList, &FilmsList::SetCurrentFilmIsFavourite );
    connect( bFavourite, &QPushButton::clicked, this, &MainWindow::UpdateCurrentFilm );
    connect( contextMenu, &FilmsViewContextMenu::actionIsFavourite, filmsList, &FilmsList::SetCurrentFilmIsFavourite );
    connect( contextMenu, &FilmsViewContextMenu::actionIsFavourite, this, &MainWindow::UpdateCurrentFilm );
    connect( contextMenu, &FilmsViewContextMenu::actionIsFavourite, bFavourite, &QPushButton::setChecked );
    // Quick search input
    connect( eFilter, &SearchEdit::TextChanged, this, &MainWindow::FilmsFilter );
    connect( filmsList, &FilmsList::DatabaseLoaded, this, &MainWindow::SetupCompleter );
    connect( filmsList, &FilmsList::DatabaseChanged, this, &MainWindow::SetupCompleter );
    // Player setup
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
    connect( editFilmWindow, &EditFilmWindow::Done, this, &MainWindow::UpdateCurrentFilm );
    connect( editFilmWindow, &EditFilmWindow::Done, this, &MainWindow::ShowFilmInformation );

    /// Remove film dialog
    connect( actionRemove, &QAction::triggered, this, &MainWindow::RemoveFilm );
    connect( toolbar, &ToolBar::actionRemove, this, &MainWindow::RemoveFilm );
    connect( contextMenu, &FilmsViewContextMenu::actionRemove, this, &MainWindow::RemoveFilm );

    /// Remove file
    connect( contextMenu, &FilmsViewContextMenu::actionRemoveFile, this, &MainWindow::RemoveFile );

    /// Film info window
#ifdef MEDIAINFO_SUPPORT
    filmInfoWindow = new FilmInfoWindow( this );

    connect( bTechInformation, &QPushButton::clicked, filmInfoWindow, &FilmInfoWindow::show );
    connect( contextMenu, &FilmsViewContextMenu::actionShowInfo, filmInfoWindow, &FilmInfoWindow::show );

    connect( filmInfoWindow, &FilmInfoWindow::ShortInfoLoaded, this, &MainWindow::ShowShortTechnicalInfo );
#endif // MEDIAINFO_SUPPORT

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
    connect( filmScannerWindow, &FilmScannerWindow::AddFilms, this, &MainWindow::DatabaseIsLoaded );

    /// Moved films window
    movedFilmsWindow = new MovedFilmsWindow( settings, this );

    connect( actionMovedFilms, &QAction::triggered, this, &MainWindow::MovedFilms );

    connect( movedFilmsWindow, &MovedFilmsWindow::FilmsMoved, this, &MainWindow::ShowFilms );
    connect( movedFilmsWindow, &MovedFilmsWindow::FilmsMoved, filmsList, &FilmsList::FilmsMoved );

    /// Statistic window
    statisticsWindow = new StatisticsWindow( this );

    connect( actionStatistics, &QAction::triggered, this, &MainWindow::Statistics );

    connect( statisticsWindow, &StatisticsWindow::ResetStatistics, this, &MainWindow::ResetStatistics );

    /// HackZone
    // There is no ability to set a shortcut on QLineEdit. So here...
    // Created button 0x0 pixels, which do stuff on shortcut
    connect( bActivateQuickSearch, &QPushButton::clicked, this, [this] { eFilter->setFocus(); } );
    connect( bSetViewFocus, &QPushButton::clicked, this, &MainWindow::QuickSearchEscBehavior );

    bSetViewFocus->setShortcut( QKeySequence( "Esc" ) );
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
    lBudget->clear();
    lCountry->clear();
    lScreenwriter->clear();
    lDirector->clear();
    lProducer->clear();
    lComposer->clear();
    lStarring->clear();
    lRating->clear();
    lDescription->clear();
    lTags->clear();
    lTechInformation->clear();
}

void MainWindow::SetAllFunctionsEnabled( bool b )
{
    toolbar->SetAllFunctionsEnabled( b );

    actionAdd->setEnabled( b );
    actionEdit->setEnabled( b );
    actionRemove->setEnabled( b );
    actionRandom->setEnabled( b );
    actionSearch->setEnabled( b );

    actionFilmScanner->setEnabled( b );
    actionMovedFilms->setEnabled( b );
    actionStatistics->setEnabled( b );

    bViewed->setEnabled( b );
    bFavourite->setEnabled( b );
    bTechInformation->setEnabled( b );
    bPlay->setEnabled( b );
    bAddToPlaylist->setEnabled( b );
}

void MainWindow::SetEmptyMode( bool b )
{
    toolbar->SetEmptyDatabaseMode( b );

    actionEdit->setDisabled( b );
    actionRemove->setDisabled( b );
    actionRandom->setDisabled( b );
    actionSearch->setDisabled( b );

    actionMovedFilms->setDisabled( b );
    actionStatistics->setDisabled( b );

    bViewed->setDisabled( b );
    bFavourite->setDisabled( b );
    bTechInformation->setDisabled( b );
    bPlay->setDisabled( b );
    bAddToPlaylist->setDisabled( b );
}

void MainWindow::SetReadOnlyMode( bool b )
{
    toolbar->SetReadOnlyMode( b );

    actionAdd->setDisabled( b );
    actionEdit->setDisabled( b );
    actionRemove->setDisabled( b );

    actionFilmScanner->setDisabled( b );
    actionMovedFilms->setDisabled( b );
    actionStatistics->setDisabled( b );

    bViewed->setDisabled( b );
    bFavourite->setDisabled( b );
}
