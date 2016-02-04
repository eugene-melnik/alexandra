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

#include "datamanip/addfilmwindow.h"
#include "datamanip/editfilmwindow.h"
#include "filmsview/grid/filmsviewgrid.h"
#include "filmsview/list/filmsviewlist.h"
#include "tools/filesextensions.h"
#include "tools/debug.h"
#include "tools/playlist.h"
#include "mainwindow.h"
#include "splashscreen.h"
#include "version.h"

#include <QDesktopWidget>
#include <QFileInfo>
#include <QMessageBox>
#include <QRect>
#include <list>


MainWindow::MainWindow() : QMainWindow(),
    settings( AlexandraSettings::GetInstance() ),
    externalPlayer( new QProcess( this ) )
{
    DebugPrintFunc( "MainWindow::MainWindow" );

    ShowSplashscreen();
    setWindowTitle( Alexandra::appNameGui );
    setupUi( this );

    SetupModels();
    SetupWindows();
    SetupFilmsView();

    LoadSettings();

    filmsListModel->LoadFromFile( settings->GetDatabaseFilePath() );
///    filmsListModel->SetCurrentFilm( settings->GetCurrentFilmTitle() );

    DebugPrintFuncDone( "MainWindow::MainWindow" );
}


void MainWindow::AddFilmsFromOutside( const QStringList& films )
{
    DebugPrintFunc( "MainWindow::AddFilmsFromOutside", films.size() );

//    QList<Film> newFilms;
//    QString messageText;

//    for( const QString& film : films )
//    {
//        QFileInfo filmInfo( film );
//        QString fileSuffix = "*." + filmInfo.suffix();

//        if( FilesExtensions().GetFilmExtensionsForDirFilter().contains( fileSuffix ) )
//        {
//            Film f;
//            f.SetFileName( film );
//            f.SetTitle( filmInfo.completeBaseName() );
//            newFilms.append( f );
//            messageText.append( QString::number( newFilms.size() ) + ") " + filmInfo.fileName() + "\n" );
//        }
//    }

//    if( newFilms.size() == 0 )
//    {
//        QMessageBox::information( this, tr( "Add films" ), tr( "There is nothing to add." ) );
//        return;
//    }

//    messageText = tr( "Add the following film(s)?\n" ) + messageText;

//    int res = QMessageBox::question( this, tr( "Add films" ), messageText,
//                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );

//    if( res == QMessageBox::Yes )
//    {
///        AddFilmsDone( &newFilms );
//        QMessageBox::information( this, tr( "Add films" ), tr( "Done!" ) );
//    }

    DebugPrintFuncDone( "MainWindow::AddFilmsFromOutside" );
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

///    eFilter->completer()->popup()->close();

    QAbstractItemView* view = dynamic_cast<QAbstractItemView*>( filmsView );

    if( view == focusWidget() )
    {
        eFilter->clear();
    }

    view->setFocus();
    filmsView->ScrollToCurrentItem();
}


void MainWindow::DatabaseChanged()
{
    statusbar->ShowTotal( filmsListModel->GetFilmsCount(),
                          filmsListModel->GetIsViewedFilmsCount(),
                          filmsListModel->GetIsFavouriteFilmsCount() );

///    filmsListModel->SaveToFileAsync( settings->GetDatabaseFilePath() );
}


//void MainWindow::ResetStatistics()
//{
//    filmsList->ResetViews();
//    QMessageBox::information( this, tr( "Reset statistics" ), tr( "Done!" ) );
//}


void MainWindow::ReloadDatabase()
{
    filmsListModel->LoadFromFile( settings->GetDatabaseFilePath() );
///    filmsView->SelectItem( settings->GetCurrentFilmTitle() );
}


void MainWindow::ReloadSettings()
{
    LoadAppearance();
    LoadShorcuts();
    toolbar->LoadSettings();
    filmsView->ReloadSettings();
}


void MainWindow::ReloadView()
{
    SetupFilmsView();
    filmsView->LoadSettings();
}


void MainWindow::DatabaseReadError( const QString& message )
{
    ClearTextFields();
    SetAllFunctionsEnabled( false );
    wFilmInfo->ShowMessage( tr( "Error reading the database!" ) );
    QMessageBox::critical( this, tr( "Database read error" ), message );
}


void MainWindow::DatabaseIsEmpty()
{
    ClearTextFields();
    SetEmptyMode();
}


void MainWindow::DatabaseIsReadonly()
{
    SetReadOnlyMode();
    QMessageBox::information( this, tr( "Database" ), tr( "Database is readonly! Editing functions are disabled." ) );
}


void MainWindow::ShowFilmInformation( const QModelIndex& index )
{
    if( index.isValid() )
    {
        const FilmItem* film = filmsListProxyModel->GetFilmItemByIndex( index );

          // Buttons
        bool isExists = film->GetIsFileExists() == FilmItem::Exists ? true : false;
        bPlay->setEnabled( isExists );
        bAddToPlaylist->setEnabled( isExists );
        bTechInformation->setEnabled( isExists );

        bViewed->setChecked( film->GetIsFilmViewed() );
        bFavourite->setChecked( film->GetIsFilmFavourite() );

          // Film info
        wFilmInfo->ShowInformation( index );
        lFilmPoster->ShowInformation( index );
        lTechInformation->ShowInformation( index );
    }
}


void MainWindow::ShowFilmContextMenu( const QPoint& pos, const QModelIndex& index )
{
    contextMenu->SetupMenuState( filmsListProxyModel->GetFilmItemByIndex(index) );
    contextMenu->exec( dynamic_cast<QWidget*>( filmsView )->mapToGlobal( pos ) );
}


//void MainWindow::AddToPlaylist()
//{
//    DebugPrintFunc( "MainWindow::AddToPlaylist" );
//    QStringList filmsTitles = filmsView->GetSelectedItemsList();

//    if( filmsTitles.size() > 0 )
//    {
//        for( QString t : filmsTitles )
//        {
//            const Film* film = filmsList->GetFilmByTitle( t );
//            lwPlaylist->AddItem( film->GetTitle(), film->GetFileName() );
//        }
//    }
//    else
//    {
//        lwPlaylist->AddItem( filmsList->GetCurrentFilmTitle(), filmsList->GetCurrentFilmFileName() );
//    }

//    bPlay->setText( tr( "Play list" ) );
//    wPlaylist->show();
//}


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
//        PlayFilm();
    }
    else if( s == "add-to-list" )
    {
//        AddToPlaylist();
    }
    else
    // "auto" mode -- plays film if playlist is empty
    // and adds film to playlist otherwise
    {
        if( lwPlaylist->IsEmpty() )
        {
//            PlayFilm();
        }
        else
        {
//            AddToPlaylist();
        }
    }
}


//void MainWindow::PlayFilm()
//{
//    DebugPrintFunc( "MainWindow::PlayFilm" );

//    if( !bPlay->isEnabled() ) return;

//    if( externalPlayer->state() == QProcess::NotRunning )
//    {
//        if( filmsView->GetSelectedItemsList().size() > 1 )
//        {
//            AddToPlaylist();
//        }

//        QString fileToPlay;

//        if( lwPlaylist->IsEmpty() )
//        {
//            fileToPlay = filmsList->GetCurrentFilmFileName();
//        }
//        else
//        {
//            fileToPlay = PlayList( lwPlaylist->GetPathes() ).CreateTempListM3U8();
//        }

//        #ifdef Q_OS_LINUX
//            externalPlayer->start( settings->GetExternalPlayer() + " \"" + fileToPlay +"\"" );
//        #elif defined(Q_OS_WIN32)
//            externalPlayer->start( "\"" + settings->GetExternalPlayer() + "\" \"" + fileToPlay +"\"" );
//        #endif
//    }
//    else
//    {
//        externalPlayer->close();
//    }
//}


void MainWindow::PlayerStarted()
{
    dynamic_cast<QWidget*>( filmsView )->setEnabled( false );
    eFilter->setEnabled( false );
    bPlay->setText( tr( "STOP" ) );

    DebugPrint( "Player started" );
}


//void MainWindow::PlayerClosed()
//{
//    dynamic_cast<QWidget*>( filmsView )->setEnabled( true );
//    eFilter->setEnabled( true );
//    bPlay->setText( tr( "&PLAY" ) );

//    // TODO: make all films of playlist is viewed
//    // need to think about this
//    if( lwPlaylist->IsEmpty() )
//    {
//        filmsList->IncCurrentFilmViewsCounter();

//        if( bViewed->isEnabled() && !bViewed->isChecked() )
//        {
//            bViewed->setChecked( true );
//            UpdateCurrentFilm();
//        }
//    }
//    else
//    {
//        lwPlaylist->Clear();
//    }

//    dynamic_cast<QWidget*>( filmsView )->setFocus();
//    DebugPrint( "Player stopped" );
//}


//void MainWindow::RemoveFilm()
//{
//    QStringList films = filmsView->GetSelectedItemsList();
//    int currentIndex = filmsView->GetCurrentItemIndex();
//    int res;

//    if( films.count() == 1 ) // One film selected
//    {
//        res = QMessageBox::question( this, tr( "Remove film" ),
//                                     tr( "Are you sure to remove \"%1\"?" ).arg( filmsList->GetCurrentFilmTitle() ),
//                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No );
//    }
//    else // More than one film selected
//    {
//        QString message = tr( "Are you sure to remove following films?\n" );
//        const int maxFilmsCount = 20;
//        int counter = 1;

//        for( int i = 0; i < std::min( films.size(), maxFilmsCount ); i++ )
//        {
//            message.append( QString( "%1) %2\n" ).arg( counter++ ).arg( films[i] ) );
//        }

//        if( films.size() > maxFilmsCount )
//        {
//            message.append( "..." );
//        }

//        res = QMessageBox::question( this, tr( "Remove film" ), message,
//                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No );
//    }

//    if( res == QMessageBox::Yes )
//    {
//        for( const QString& s : films )
//        {
//            filmsList->RemoveFilmByTitle( s );
//        }

//        SaveDatabase();
//        filmsView->SetCurrentItemIndex( currentIndex );
//    }
//}


//void MainWindow::RemoveFile()
//{
//    int res = QMessageBox::question( this, tr( "Remove file" ),
//                                     tr( "Are you sure to remove file \"%1\"?" ).arg( filmsList->GetCurrentFilmFileName() ),
//                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

//    if( res == QMessageBox::Yes )
//    {
//        if( QFile( filmsList->GetCurrentFilmFileName() ).remove() )
//        {
//            res = QMessageBox::question( this, tr( "Remove file" ), tr( "Remove record from database?" ),
//                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

//            if( res == QMessageBox::Yes )
//            {
//                filmsList->RemoveCurrentFilm();
//                filmsView->RemoveCurrentItem();
//            }
//        }
//        else
//        {
//            QMessageBox::warning( this, tr( "Remove file" ),
//                                  tr( "Unable to remove file \"%1\"!" ).arg( filmsList->GetCurrentFilmFileName() ) );
//        }
//    }
//}


//void MainWindow::UpdateCurrentFilm( Film film )
//{
//    QString newCurrentTitle = film.GetTitle();
//    DebugPrintFunc( "MainWindow::UpdateCurrentFilm", newCurrentTitle );

//    if( !newCurrentTitle.isEmpty() ) // if not just update view
//    {
//        QStringList titles = filmsList->GetTitlesList();

//        if( newCurrentTitle != filmsList->GetCurrentFilmTitle()
//                && titles.contains( newCurrentTitle )  )
//        {
//            film.SetTitle( newCurrentTitle + tr( " (another)") );
//        }

//        filmsList->ChangeCurrentFilm( film );
//        DebugPrint( "Film information changed." );
//    }

//    if( filmsList->GetCurrentFilm() != nullptr )
//    {
//        filmsView->SetCurrentItemTo( *filmsList->GetCurrentFilm() );
//    }
//}


void MainWindow::ShowAddFilmWindow()
{
    AddFilmWindow* addFilmWindow = new AddFilmWindow( this );
    connect( addFilmWindow, &AddFilmWindow::Done, this, &MainWindow::AddFilmDone );

    addFilmWindow->show();
}

void MainWindow::ShowEditFilmWindow()
{
    EditFilmWindow* editFilmWindow = new EditFilmWindow( this );
    connect( editFilmWindow, &EditFilmWindow::Done, this, &MainWindow::EditFilmDone );

    editFilmWindow->SetData( filmsListProxyModel->GetFilmItemByIndex( filmsView->GetCurrentIndex() ) );
    editFilmWindow->show();
}


void MainWindow::AddFilmDone( FilmItem* film )
{
    filmsListModel->AddFilmItem( film );

///    SaveDatabase(); // => on DataChanged (?)
///    SetAllFunctionsEnabled( true ); // if model was empty
}

void MainWindow::EditFilmDone( FilmItem* film )
{
    filmsListModel->EditFilmItem( film, filmsListProxyModel->mapToSource(filmsView->GetCurrentIndex()) );
    //delete film;
}


//void MainWindow::AddFilmsDone( const QList<Film>* films )
//{
//    QStringList titles = filmsList->GetTitlesList();

//    for( Film film : *films )
//    {
//        while( titles.contains( film.GetTitle() ) )
//        {
//            film.SetTitle( film.GetTitle() + tr( " (another)") );
//        }

//        filmsList->AddFilm( film );
//        titles.append( film.GetTitle() );
//    }

//    SaveDatabase();
//    SetAllFunctionsEnabled( true );
//    ShowFilms();
//}


void MainWindow::ShowSplashscreen()
{
    if( settings->GetApplicationShowSplashScreen() )
    {
        SplashScreen* splashScreen = new SplashScreen();
        splashScreen->show();
        qApp->processEvents();
    }
}


void MainWindow::SetupModels()
{
    DebugPrintFunc( "MainWindow::SetupModels" );

    filmsListModel = new FilmsListModel( this );
    filmsListProxyModel = new FilmsListProxyModel( this );
    filmsListProxyModel->setSourceModel( filmsListModel );

    DebugPrintFuncDone( "MainWindow::SetupModels" );
}


void MainWindow::SetupWindows()
{
    DebugPrintFunc( "MainWindow::SetupWindows" );

        /// Main window
    contextMenu = new FilmsViewContextMenu( this );

    connect( actionShowFullscreen, &QAction::toggled,    this, &MainWindow::ShowFullScreen );
    connect( toolbar,              &ToolBar::actionExit, this, &MainWindow::close );

    connect( filmsListModel, &FilmsListModel::DatabaseLoaded, this, &MainWindow::DatabaseIsLoaded );
    connect( filmsListModel, &FilmsListModel::DatabaseReadError,  this, &MainWindow::DatabaseReadError );
    connect( filmsListModel, &FilmsListModel::DatabaseIsReadonly, this, &MainWindow::DatabaseIsReadonly );
    connect( filmsListModel, &FilmsListModel::DatabaseIsEmpty,    this, &MainWindow::DatabaseIsEmpty );
    connect( filmsListModel, &FilmsListModel::DatabaseChanged, this, &MainWindow::DatabaseChanged );

    eFilter->SetModel( filmsListProxyModel );
    connect( eFilter, &SearchEdit::TextChanged, filmsListProxyModel, &FilmsListProxyModel::SetFilter );

      // Playlist
//    connect( bAddToPlaylist, &QPushButton::clicked, this, &MainWindow::AddToPlaylist );
//    connect( contextMenu, &FilmsViewContextMenu::actionAddToList, this, &MainWindow::AddToPlaylist );
//    connect( lwPlaylist, &PlayListWidget::Cleared, this, &MainWindow::PlaylistCleared );
    wPlaylist->hide();

      // Play button
//    connect( bPlay, &QPushButton::clicked, this, &MainWindow::PlayFilm );
//    connect( contextMenu, &FilmsViewContextMenu::actionPlay, this, &MainWindow::PlayFilm );
      // Viewed button
//    connect( bViewed, &QPushButton::clicked, filmsListModel, &FilmsListModel::SetCurrentFilmIsViewed );
//    connect( bViewed, SIGNAL( clicked() ), this, SLOT( UpdateCurrentFilm() ) );
//    connect( contextMenu, &FilmsViewContextMenu::actionIsViewed, filmsListModel, &FilmsListModel::SetCurrentFilmIsViewed );
//    connect( contextMenu, SIGNAL( actionIsViewed(bool) ), this, SLOT( UpdateCurrentFilm() ) );
//    connect( contextMenu, &FilmsViewContextMenu::actionIsViewed, bViewed, &QPushButton::setChecked );
      // Favourite button
//    connect( bFavourite, &QPushButton::clicked, filmsListModel, &FilmsListModel::SetCurrentFilmIsFavourite );
//    connect( bFavourite, SIGNAL( clicked() ), this, SLOT( UpdateCurrentFilm() ) );
//    connect( contextMenu, &FilmsViewContextMenu::actionIsFavourite, filmsListModel, &FilmsListModel::SetCurrentFilmIsFavourite );
//    connect( contextMenu, SIGNAL( actionIsFavourite(bool) ), this, SLOT( UpdateCurrentFilm() ) );
//    connect( contextMenu, &FilmsViewContextMenu::actionIsFavourite, bFavourite, &QPushButton::setChecked );
      // Quick search input
//    connect( eFilter, &SearchEdit::TextChanged, this, &MainWindow::FilmsFilter );
//    connect( filmsListModel, &FilmsListModel::DatabaseLoaded, this, &MainWindow::SetupCompleter );
//    connect( filmsListModel, &FilmsListModel::DatabaseChanged, this, &MainWindow::SetupCompleter );
      // Player setup
//    connect( externalPlayer, &QProcess::started, this, &MainWindow::PlayerStarted );
//    connect( externalPlayer, SIGNAL( finished(int) ), this, SLOT( PlayerClosed() ) );

        /// About window
    aboutWindow = new AboutWindow( this );
    connect( actionAbout,   &QAction::triggered, aboutWindow, &AboutWindow::show );
    connect( actionAboutQt, &QAction::triggered, aboutWindow, &AboutWindow::AboutQt );

        /// Add film window
    connect( actionAdd, &QAction::triggered, this, &MainWindow::ShowAddFilmWindow );
    connect( toolbar,   &ToolBar::actionAdd, this, &MainWindow::ShowAddFilmWindow );

        /// Edit film window
    connect( actionEdit,  &QAction::triggered,               this, &MainWindow::ShowEditFilmWindow );
    connect( toolbar,     &ToolBar::actionEdit,              this, &MainWindow::ShowEditFilmWindow );
    connect( contextMenu, &FilmsViewContextMenu::actionEdit, this, &MainWindow::ShowEditFilmWindow );


        /// Remove film dialog
//    connect( actionRemove, &QAction::triggered, this, &MainWindow::RemoveFilm );
//    connect( toolbar, &ToolBar::actionRemove, this, &MainWindow::RemoveFilm );
//    connect( contextMenu, &FilmsViewContextMenu::actionRemove, this, &MainWindow::RemoveFilm );

        /// Remove file
//    connect( contextMenu, &FilmsViewContextMenu::actionRemoveFile, this, &MainWindow::RemoveFile );

        /// Film info window
//#ifdef MEDIAINFO_SUPPORT
//    filmInfoWindow = new FilmInfoWindow( this );

//    connect( bTechInformation, &QPushButton::clicked, filmInfoWindow, &FilmInfoWindow::show );
//    connect( contextMenu, &FilmsViewContextMenu::actionShowInfo, filmInfoWindow, &FilmInfoWindow::show );
//#endif // MEDIAINFO_SUPPORT

        /// Search window
//    searchWindow = new SearchWindow( filmsListModel->GetFilmsList(), this );

//    connect( actionSearch, &QAction::triggered, searchWindow, &SearchWindow::show );
//    connect( toolbar, &ToolBar::actionSearch, searchWindow, &SearchWindow::show );

//    connect( searchWindow, &SearchWindow::FilmSelected, filmsListModel, &FilmsListModel::SetCurrentFilm );

        /// Settings window
    settingsWindow = new SettingsWindow( this );
    connect( actionSettings, &QAction::triggered, settingsWindow, &SettingsWindow::show );

    connect( settingsWindow, &SettingsWindow::SettingsChanged,         this,           &MainWindow::ReloadSettings );
    connect( settingsWindow, &SettingsWindow::ViewChanged,             this,           &MainWindow::ReloadView );
    connect( settingsWindow, &SettingsWindow::DatabaseSettingsChanged, this,           &MainWindow::ReloadDatabase );
    connect( settingsWindow, &SettingsWindow::EraseDatabase,           filmsListModel, &FilmsListModel::EraseAll );

        /// Film scanner window
//    filmScannerWindow = new FilmScannerWindow( this );

//    connect( actionFilmScanner, &QAction::triggered, this, &MainWindow::FilmScanner );
//    connect( toolbar, &ToolBar::actionFilmScanner, this, &MainWindow::FilmScanner );

//    connect( filmScannerWindow, &FilmScannerWindow::AddFilms, this, &MainWindow::AddFilmsDone );

        /// Moved films window
//    movedFilmsWindow = new MovedFilmsWindow( this );

//    connect( actionMovedFilms, &QAction::triggered, this, &MainWindow::MovedFilms );

//    connect( movedFilmsWindow, &MovedFilmsWindow::FilmsMoved, this, &MainWindow::ShowFilms );
//    connect( movedFilmsWindow, &MovedFilmsWindow::FilmsMoved, filmsListModel, &FilmsListModel::FilmsMoved );

        /// Statistic window
//    statisticsWindow = new StatisticsWindow( this );
//    connect( actionStatistics, &QAction::triggered, this, &MainWindow::Statistics );
//    connect( statisticsWindow, &StatisticsWindow::ResetStatistics, this, &MainWindow::ResetStatistics );

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


void MainWindow::SetupFilmsView()
{
    DebugPrintFunc( "MainWindow::SetupFilmsView" );

    eFilter->clear();
    delete filmsView;

    QAbstractItemView* view;

    switch( settings->GetFilmsViewMode() )
    {
        case Alexandra::GridMode :
        {
            filmsListProxyModel->sort( FilmItem::TitleColumn );
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

    filmsView = dynamic_cast<AbstractFilmsView*>( view );

      // Base signals
    connect( view, SIGNAL(CurrentChanged(QModelIndex)), this, SLOT(ShowFilmInformation(QModelIndex)) );
    connect( view, SIGNAL( ContextMenuRequested(QPoint,QModelIndex) ), this, SLOT( ShowFilmContextMenu(QPoint,QModelIndex) ) );
      // Search window
//    connect( searchWindow, SIGNAL( FilmSelected(QString) ), view, SLOT( SelectItem(QString) ) );
      // Random film function
//    connect( actionRandom, SIGNAL( triggered() ), view, SLOT( SelectRandomItem() ) );
//    connect( toolbar, SIGNAL( actionRandom() ), view, SLOT( SelectRandomItem() ) );

    view->setModel( filmsListProxyModel );
    vlLeft->insertWidget( 0, view );
    view->show();
    view->setFocus();

    DebugPrintFuncDone( "MainWindow::SetupFilmsView" );
}


void MainWindow::LoadSettings()
{
    DebugPrintFunc( "MainWindow::LoadSettings" );

///    QPixmapCache::setCacheLimit();

    LoadAppearance();
    LoadShorcuts();

      // Main window
    if( settings->GetMainWindowGeometry().isEmpty() )
    {
          // On first launch (or without settings) move the window
          // to the center of the screen
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

    DebugPrintFuncDone( "MainWindow::LoadSettings" );
}


void MainWindow::LoadAppearance()
{
      // Font
    QFont font;
    font.fromString( settings->GetApplicationFont() );

    qApp->setFont( font );
    wFilmInfo->setFont( font );
    statusbar->setFont( font );

      // Style
    QString style = settings->GetApplicationStyleName();

    if( style == tr( "<Theme>" ) )
    {
        QFile file( Alexandra::themes[ settings->GetApplicationThemeIndex() ].path + "style.qss" );
        file.open( QIODevice::ReadOnly );
        qApp->setStyleSheet( QString::fromUtf8( file.readAll() ) );
        file.close();

        DebugPrint( "Theme loaded: " + file.fileName() );
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
///    settings->SetCurrentFilmTitle( filmsList->GetCurrentFilmTitle() );

    settings->sync();
    DebugPrintFuncDone( "MainWindow::SaveSettings" );
}


void MainWindow::ClearTextFields()
{
    wFilmInfo->Clear();
    lFilmPoster->Clear();
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

    wFilmInfo->ShowEmptyDatabaseMessage(); // TODO: check in FilmInfoView (?)
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

