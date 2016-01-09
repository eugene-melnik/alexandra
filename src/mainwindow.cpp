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

#include "filmsview/filmsviewgrid.h"
#include "filmsview/filmsviewlist.h"
#include "tools/filesextensions.h"
#include "tools/debug.h"
#include "mainwindow.h"
#include "playlist.h"
#include "version.h"

#include <QCompleter>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QMessageBox>
#include <QRect>
#include <list>

MainWindow::MainWindow() : QMainWindow()
{
    DebugPrintFunc( "MainWindow::MainWindow" );
    settings = AlexandraSettings::GetInstance();

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
    DebugPrintFuncA( "MainWindow::AddFilmsFromOutside", films.size() );

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
        AddFilmsDone( &newFilms );
        QMessageBox::information( this, tr( "Add films" ), tr( "Done!" ) );
    }
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    DebugPrintFunc( "MainWindow::closeEvent" );

    hide();
    SaveSettings();
    event->accept();
}

void MainWindow::QuickSearchEscBehavior()
{
    // Pressing key 'Esc' sets focus to the films list,
    // pressing again clears the quick search line

    eFilter->completer()->popup()->close();

    QWidget* view = dynamic_cast<QWidget*>( filmsView );

    if( view == focusWidget() )
    {
        eFilter->clear();
    }

    view->setFocus();
}

void MainWindow::SaveDatabase()
{
    SetupCompleter();
    StatusbarShowTotal();
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
    }
}

void MainWindow::ResetStatistics()
{
    filmsList->ResetViews();
    QMessageBox::information( this, tr( "Reset statistics" ), tr( "Done!" ) );
}

void MainWindow::ReloadDatabase()
{
    DebugPrintFunc( "MainWindow::ReloadDatabase" );

    filmsList->LoadFromFile( settings->GetDatabaseFilePath() );
    filmsView->SelectItem( settings->GetCurrentFilmTitle() );
}

void MainWindow::ReloadSettings()
{
    DebugPrintFunc( "MainWindow::ReloadSettings" );

    LoadAppearance();
    LoadShorcuts();

    toolbar->LoadSettings();
    filmsView->ReloadSettings();

    ShowFilmInformation();
}

void MainWindow::ReloadView()
{
    DebugPrintFunc( "MainWindow::ReloadView" );

    SetupFilmsView();
    filmsView->LoadSettings();
    ShowFilms();
}

void MainWindow::DatabaseReadError()
{
    ClearTextFields();
    SetAllFunctionsEnabled( false );
    wFilmInfo->ShowMessage( tr( "Error reading the database!" ) );

    QMessageBox::critical( this, tr( "Database" ),
                                 tr( "Error reading the database! Check the permissions or choose another "
                                     "database file in settings (\"Application\" tab)." ) );
}

void MainWindow::DatabaseIsEmpty()
{
    ClearTextFields();
    SetEmptyMode();
}

void MainWindow::DatabaseIsReadonly()
{
    SetReadOnlyMode();

    QMessageBox::information( this, tr( "Database" ),
                                    tr( "Database is readonly! Editing functions are disabled." ) );
}

void MainWindow::ShowFilms()
{
    DebugPrintFunc( "MainWindow::ShowFilms" );

    SetProgressMaximum( filmsList->GetFilmsCount() );
    statusbar->ShowLoading();

    int currentIndex = filmsView->GetCurrentItemIndex();
    filmsView->Clear();

    const QList<Film>* films = filmsList->GetFilmsList();

    for( int i = 0; i < films->size(); i++ )
    {
        filmsView->AddItem( films->at(i) );
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
    DebugPrintFuncDone( "MainWindow::ShowFilms" );

    emit Shown();
}

void MainWindow::ShowFilmInformation()
{
    DebugPrintFunc( "MainWindow::ShowFilmInformation" );
    const Film* film = filmsList->GetCurrentFilm();

    if( film == nullptr )
    {
        return;
    }

    DebugPrint( "Selected film: " + film->GetFileName() );

    // Buttons and technical information
    if( QFile::exists( film->GetFileName() ) )
    {
        #ifdef MEDIAINFO_SUPPORT
            filmInfoWindow->LoadTechnicalInfoAsync( film->GetFileName() );
            bTechInformation->setEnabled( true );
        #endif

        bPlay->setEnabled( true );
        bAddToPlaylist->setEnabled( true );
    }
    else
    {
        // File doesn't exists
        bPlay->setEnabled( false );
        bAddToPlaylist->setEnabled( false );
        bTechInformation->setEnabled( false );
    }

    // Main information
    wFilmInfo->ShowFilmInfo( film );

    lTechInformation->setVisible( false );

    // Buttons
    bViewed->setChecked( film->GetIsViewed() );
    bFavourite->setChecked( film->GetIsFavourite() );

    // Poster
    QPixmap p;

    if( film->GetIsPosterExists() )
    {
        QString posterFileName = settings->GetPostersDirPath() + "/" + film->GetPosterName();
        DebugPrint( "Loading poster: " + posterFileName );
        p.load( posterFileName );
    }

    if( p.isNull() )
    {
        DebugPrint( "Loading standart poster" );
        p.load( ":/standart-poster" );
    }

    p = p.scaledToWidth( wRight->maximumWidth(), Qt::SmoothTransformation );
    lPosterImage->setPixmap( p );

    DebugPrintFuncDone( "MainWindow::ShowFilmInformation" );
}

void MainWindow::ShowFilmContextMenu( QPoint p )
{
    contextMenu->SetState( filmsList->GetCurrentFilm() );
    contextMenu->exec( dynamic_cast<QWidget*>( filmsView )->mapToGlobal( p ) );
}

void MainWindow::ShowShortTechnicalInfo( const QString& info )
{
    DebugPrintFunc( "MainWindow::ShowShortTechnicalInfo" );
    lTechInformation->setText( info );
    lTechInformation->setVisible( true );
}

void MainWindow::AddToPlaylist()
{
    DebugPrintFunc( "MainWindow::AddToPlaylist" );
    QStringList filmsTitles = filmsView->GetSelectedItemsList();

    if( filmsTitles.size() > 0 )
    {
        for( QString t : filmsTitles )
        {
            const Film* film = filmsList->GetFilmByTitle( t );
            lwPlaylist->AddItem( film->GetTitle(), film->GetFileName() );
        }
    }
    else
    {
        lwPlaylist->AddItem( filmsList->GetCurrentFilmTitle(),
                             filmsList->GetCurrentFilmFileName() );
    }

    bPlay->setText( tr( "Play list" ) );
    wPlaylist->show();
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

void MainWindow::PlayFilm()
{
    DebugPrintFunc( "MainWindow::PlayFilm" );

    if( !bPlay->isEnabled() ) return;

    if( externalPlayer->state() == QProcess::NotRunning )
    {
        if( filmsView->GetSelectedItemsList().size() > 1 )
        {
            AddToPlaylist();
        }

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

    DebugPrint( "Player started" );
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

    DebugPrint( "Player stopped" );
}

void MainWindow::RemoveFilm()
{
    QStringList films = filmsView->GetSelectedItemsList();
    int currentIndex = filmsView->GetCurrentItemIndex();
    int res;

    if( films.count() == 1 ) // One film selected
    {
        res = QMessageBox::question( this, tr( "Remove film" ),
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

        res = QMessageBox::question( this, tr( "Remove film" ),
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

        SaveDatabase();
        filmsView->SetCurrentItemIndex( currentIndex );
    }
}

void MainWindow::RemoveFile()
{
    int res = QMessageBox::question( this, tr( "Remove file" ),
                                           tr( "Are you sure to remove file \"%1\"?" ).arg( filmsList->GetCurrentFilmFileName() ),
                                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

    if( res == QMessageBox::Yes )
    {
        if( QFile( filmsList->GetCurrentFilmFileName() ).remove() )
        {
            res = QMessageBox::question( this, tr( "Remove file" ),
                                               tr( "Remove record from database?" ),
                                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

            if( res == QMessageBox::Yes )
            {
                filmsList->RemoveCurrentFilm();
                filmsView->RemoveCurrentItem();
            }
        }
        else
        {
            QMessageBox::warning( this, tr( "Remove file" ),
                                        tr( "Unable to remove file \"%1\"!" ).arg( filmsList->GetCurrentFilmFileName() ) );
        }
    }
}

void MainWindow::SetupCompleter()
{
    DebugPrintFunc( "MainWindow::SetupCompleter" );

    delete eFilter->completer();

    QCompleter* c = new QCompleter( filmsList->GetTitlesList(), this );
    c->setCaseSensitivity( Qt::CaseInsensitive );
    eFilter->setCompleter( c );

    DebugPrintFuncDone( "MainWindow::SetupCompleter" );
}

void MainWindow::FilmsFilter( const QString& key, SearchEdit::FilterBy filters )
{
    DebugPrintFuncA( "MainWindow::FilmsFilter", key );

    if( key.isEmpty() )
    {
        DebugPrint( "Filter cleared" );
        filmsView->ResetFilter();
        return;
    }

    // Filtering
    statusbar->ShowLoading();

    const QList<Film>* films = filmsList->GetFilmsList();
    std::list<QString> filteredFilms;

    for( const Film& film : *films ) // TODO: Move algorithm to the separate Search engine class
    {
        if( ( filters & SearchEdit::Title ) && film.GetTitle().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film.GetTitle() );

        if( ( filters & SearchEdit::Tags ) && film.GetTags().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film.GetTitle() );

        if( ( filters & SearchEdit::Genre ) && film.GetGenre().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film.GetTitle() );

        if( ( filters & SearchEdit::Starring ) && film.GetStarring().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film.GetTitle() );

        if( ( filters & SearchEdit::Director ) && film.GetDirector().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film.GetTitle() );

        if( ( filters & SearchEdit::Producer ) && film.GetProducer().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film.GetTitle() );

        if( ( filters & SearchEdit::Screenwriter ) && film.GetScreenwriter().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film.GetTitle() );

        if( ( filters & SearchEdit::Composer ) && film.GetComposer().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film.GetTitle() );

        if( ( filters & SearchEdit::Country ) && film.GetCountry().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film.GetTitle() );

        if( ( filters & SearchEdit::Description ) && film.GetDescription().contains( key, Qt::CaseInsensitive ) )
            filteredFilms.push_back( film.GetTitle() );
    }

    filteredFilms.sort();
    filteredFilms.unique();

    // Show founded
    filmsView->FilterBy( QStringList::fromStdList( filteredFilms ) );
    filmsView->SelectRandomItem();

    if( filteredFilms.empty() )
    {
        ClearTextFields();
        statusbar->ShowFounded( 0 );
        wFilmInfo->ShowMessage( tr( "Nothing was found! Try to change keyword or search settings." ) );
    }
    else
    {
        statusbar->ShowFounded( filteredFilms.size() );
    }

    filmsView->SetCurrentItemIndex( 0 );
    DebugPrint( "Filtered" );
}

void MainWindow::UpdateCurrentFilm( Film film )
{
    QString newCurrentTitle = film.GetTitle();
    DebugPrintFuncA( "MainWindow::UpdateCurrentFilm", newCurrentTitle );

    if( !newCurrentTitle.isEmpty() ) // if not just update view
    {
        QStringList titles = filmsList->GetTitlesList();

        if( newCurrentTitle != filmsList->GetCurrentFilmTitle()
                && titles.contains( newCurrentTitle )  )
        {
            film.SetTitle( newCurrentTitle + tr( " (another)") );
        }

        filmsList->ChangeCurrentFilm( film );
        DebugPrint( "Film information changed." );
    }

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

void MainWindow::AddFilmDone( Film film )
{
    if( filmsList->GetTitlesList().contains( film.GetTitle() ) )
    {
        film.SetTitle( film.GetTitle() + tr( " (another)") );
    }

    filmsList->AddFilm( film );
    filmsView->AddItem( film );
    filmsView->SelectItem( film );

    SaveDatabase();
    SetAllFunctionsEnabled( true );
}

void MainWindow::AddFilmsDone( const QList<Film>* films )
{
    QStringList titles = filmsList->GetTitlesList();

    for( Film film : *films )
    {
        while( titles.contains( film.GetTitle() ) )
        {
            film.SetTitle( film.GetTitle() + tr( " (another)") );
        }

        filmsList->AddFilm( film );
        titles.append( film.GetTitle() );
    }

    SaveDatabase();
    SetAllFunctionsEnabled( true );
    ShowFilms();
}

void MainWindow::LoadSettings()
{
    DebugPrintFunc( "MainWindow::LoadSettings" );

    LoadAppearance();
    LoadShorcuts();

    // Main window
    if( settings->GetMainWindowGeometry().isEmpty() )
    {
        // On first launch move the window to the center of the screen
        QRect rect = frameGeometry();
        rect.moveCenter( QDesktopWidget().availableGeometry().center() );
        move( rect.topLeft() );
    }
    else
    {
        restoreGeometry( settings->GetMainWindowGeometry() );
    }

    restoreState( settings->GetMainWindowState() );
    mainSplitter->restoreState( settings->GetMainWindowSplitterState() );
    actionShowFullscreen->setChecked( isFullScreen() );
    actionShowToolbar->setChecked( toolbar->isVisibleTo( this ) );

    statusbar->setVisible( settings->GetMainWindowStatusbarIsVisible() );
    actionShowStatusbar->setChecked( statusbar->isVisibleTo( this ) );

    // Widgets
    toolbar->LoadSettings();
    eFilter->LoadSettings();
    filmsView->LoadSettings();
}

void MainWindow::LoadAppearance()
{
    // Font
    QFont font;
    font.fromString( settings->GetApplicationFont() );

    qApp->setFont( font );
    statusbar->setFont( font );

    // Style
    QString style = settings->GetApplicationStyleName();

    if( style == tr( "<Theme>" ) )
    {
        QFile f( Alexandra::themes[ settings->GetApplicationThemeIndex() ].path + "style.qss" );

        f.open( QFile::ReadOnly );
        qApp->setStyleSheet( QString::fromUtf8( f.readAll() ) );
        f.close();

        DebugPrint( "Theme loaded: " + f.fileName() );
    }
    else if( !style.isEmpty() )
    {
        qApp->setStyleSheet( "" );
        qApp->setStyle( style );

        DebugPrint( "Style loaded: " + style );
    }

    wRight->setVisible( settings->GetMainWindowShowRightPanel() );
    wRight->setMaximumWidth( settings->GetMainWindowRightPanelWidth() );
}

void MainWindow::LoadShorcuts()
{
    actionAdd->setShortcut( QKeySequence( settings->GetShortcutAddFilm() ) );
    actionEdit->setShortcut( QKeySequence( settings->GetShortcutEditFilm() ) );
    actionRemove->setShortcut( QKeySequence( settings->GetShortcutRemoveFilm() ) );

    actionRandom->setShortcut( QKeySequence( settings->GetShortcutSelectRandomFilm() ) );
    quickSearchShortcut->setKey( QKeySequence( settings->GetShortcutActivateQuickSearch() ) );
    actionSearch->setShortcut( QKeySequence( settings->GetShortcutAdvancedSearch() ) );

    bPlay->setShortcut( QKeySequence( settings->GetShortcutPlay() ) );

    actionSettings->setShortcut( QKeySequence( settings->GetShortcutSettings() ) );
    actionShowToolbar->setShortcut( QKeySequence( settings->GetShortcutShowToolbar() ) );
    actionShowFullscreen->setShortcut( QKeySequence( settings->GetShortcutShowFullscreen() ) );

    actionExit->setShortcut( QKeySequence( settings->GetShortcutExit() ) );
}

void MainWindow::SaveSettings()
{
    DebugPrintFunc( "MainWindow::SaveSettings" );

    // Main window
    settings->SetMainWindowState( saveState() );
    settings->SetMainWindowGeometry( saveGeometry() );
    settings->SetMainWindowSplitterState( mainSplitter->saveState() );
    settings->SetMainWindowStatusbarIsVisible( statusbar->isVisibleTo( this ) );

    // Widgets
    eFilter->SaveSettings();
    filmsView->SaveSettings();

    // Choosen film
    settings->SetCurrentFilmTitle( filmsList->GetCurrentFilmTitle() );

    settings->sync();
}

void MainWindow::SetupFilmsView()
{
    DebugPrintFunc( "MainWindow::SetupFilmsView" );

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
            DebugPrint( "Grid view mode" );
            break;
        }

        // List view by default
        default :
        {
            view = new FilmsViewList( this );
            DebugPrint( "List view mode" );
        }
    }

    // Base signals
    connect( view, SIGNAL( ItemClicked(QString) ), filmsList, SLOT( SetCurrentFilm(QString) ) );
    connect( view, SIGNAL( ItemClicked(QString) ), this, SLOT( ShowFilmInformation() ) );
    connect( view, SIGNAL( ItemDoubleClicked(QString) ), this, SLOT( DoubleClickBehavior() ) );
    connect( view, SIGNAL( ContextMenuRequested(QPoint) ), this, SLOT( ShowFilmContextMenu(QPoint) ) );
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

    DebugPrintFuncDone( "MainWindow::SetupFilmsView" );
}

void MainWindow::SetupWindows()
{
    DebugPrintFunc( "MainWindow::SetupWindows" );

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
    filmsList = new FilmsList( this );

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
    connect( bViewed, SIGNAL( clicked() ), this, SLOT( UpdateCurrentFilm() ) );
    connect( contextMenu, &FilmsViewContextMenu::actionIsViewed, filmsList, &FilmsList::SetCurrentFilmIsViewed );
    connect( contextMenu, SIGNAL( actionIsViewed(bool) ), this, SLOT( UpdateCurrentFilm() ) );
    connect( contextMenu, &FilmsViewContextMenu::actionIsViewed, bViewed, &QPushButton::setChecked );
    // Favourite button
    connect( bFavourite, &QPushButton::clicked, filmsList, &FilmsList::SetCurrentFilmIsFavourite );
    connect( bFavourite, SIGNAL( clicked() ), this, SLOT( UpdateCurrentFilm() ) );
    connect( contextMenu, &FilmsViewContextMenu::actionIsFavourite, filmsList, &FilmsList::SetCurrentFilmIsFavourite );
    connect( contextMenu, SIGNAL( actionIsFavourite(bool) ), this, SLOT( UpdateCurrentFilm() ) );
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
    addFilmWindow = new AddFilmWindow( this );

    connect( actionAdd, &QAction::triggered, addFilmWindow, &AddFilmWindow::show );
    connect( toolbar, &ToolBar::actionAdd, addFilmWindow, &AddFilmWindow::show );

    connect( addFilmWindow, &AddFilmWindow::Done, this, &MainWindow::AddFilmDone );

    /// Edit film window
    editFilmWindow = new EditFilmWindow( this );

    connect( actionEdit, &QAction::triggered, this, &MainWindow::EditFilm );
    connect( toolbar, &ToolBar::actionEdit, this, &MainWindow::EditFilm );
    connect( contextMenu, &FilmsViewContextMenu::actionEdit, this, &MainWindow::EditFilm );

    connect( editFilmWindow, SIGNAL( Done(Film) ), this, SLOT( UpdateCurrentFilm(Film) ) );
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
    settingsWindow = new SettingsWindow( this );
    connect( actionSettings, &QAction::triggered, settingsWindow, &SettingsWindow::show );

    connect( settingsWindow, &SettingsWindow::SettingsChanged, this, &MainWindow::ReloadSettings );
    connect( settingsWindow, &SettingsWindow::ViewChanged, this, &MainWindow::ReloadView );
    connect( settingsWindow, &SettingsWindow::DatabaseSettingsChanged, this, &MainWindow::ReloadDatabase );
    connect( settingsWindow, &SettingsWindow::EraseDatabase, this, &MainWindow::EraseDatabase );

    /// Film scanner window
    filmScannerWindow = new FilmScannerWindow( this );

    connect( actionFilmScanner, &QAction::triggered, this, &MainWindow::FilmScanner );
    connect( toolbar, &ToolBar::actionFilmScanner, this, &MainWindow::FilmScanner );

    connect( filmScannerWindow, &FilmScannerWindow::AddFilms, this, &MainWindow::AddFilmsDone );

    /// Moved films window
    movedFilmsWindow = new MovedFilmsWindow( this );

    connect( actionMovedFilms, &QAction::triggered, this, &MainWindow::MovedFilms );

    connect( movedFilmsWindow, &MovedFilmsWindow::FilmsMoved, this, &MainWindow::ShowFilms );
    connect( movedFilmsWindow, &MovedFilmsWindow::FilmsMoved, filmsList, &FilmsList::FilmsMoved );

    /// Statistic window
    statisticsWindow = new StatisticsWindow( this );
    connect( actionStatistics, &QAction::triggered, this, &MainWindow::Statistics );
    connect( statisticsWindow, &StatisticsWindow::ResetStatistics, this, &MainWindow::ResetStatistics );

    /// Shortcuts
    quickSearchShortcut = new QShortcut( this );
    quickSearchShortcut->setContext( Qt::ApplicationShortcut );
    connect( quickSearchShortcut, SIGNAL( activated() ), eFilter, SLOT( setFocus() ) );

    viewFocusShortcut = new QShortcut( this );
    viewFocusShortcut->setKey( QKeySequence( "Esc" ) );
    viewFocusShortcut->setContext( Qt::ApplicationShortcut );
    connect( viewFocusShortcut, &QShortcut::activated, this, &MainWindow::QuickSearchEscBehavior );

    DebugPrintFuncDone( "MainWindow::SetupWindows" );
}

void MainWindow::ClearTextFields()
{
    wFilmInfo->Clear();
    lTechInformation->clear();
    lPosterImage->setPixmap( QPixmap( ":/standart-poster" ).scaledToWidth( wRight->maximumWidth(),Qt::SmoothTransformation ) );
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

    wFilmInfo->ShowEmptyDatabaseMessage();
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
