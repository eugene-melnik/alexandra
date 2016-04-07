/*************************************************************************************************
 *                                                                                                *
 *  file: mainwindow.cpp                                                                          *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2016 Eugene Melnik <jeka7js@gmail.com>                                     *
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
#include "search/searchwindow.h"
#include "scanner/filmscannerwindow.h"
#include "scanner/movedfilmswindow.h"
#include "statistics/statisticswindow.h"
#include "tools/filesextensions.h"
#include "tools/debug.h"
#include "tools/playlist.h"
#include "checkvieweddialog.h"
#include "filminfowindow.h"
#include "mainwindow.h"
#include "settingswindow.h"
#include "splashscreen.h"
#include "version.h"

#include <QDesktopWidget>
#include <QFileDialog>
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
    DebugPrintFuncDone( "MainWindow::MainWindow" );
}


void MainWindow::show()
{
    QMainWindow::show();
    SetCurrentFilmByTitle( settings->GetCurrentFilmTitle() );
}


void MainWindow::AddFilmsFromOutside( const QStringList& filmsFileNames )
{
    DebugPrintFunc( "MainWindow::AddFilmsFromOutside", filmsFileNames.size() );

    FilmItemList newFilms;
    QString messageText;

    for( const QString& filmFileName : filmsFileNames )
    {
        QFileInfo filmInfo( filmFileName );
        QString fileSuffix = "*." + filmInfo.suffix();

        if( FilesExtensions::GetFilmExtensionsForDirFilter().contains( fileSuffix ) )
        {
            QString filmTitle = filmInfo.completeBaseName();
            messageText.append( QString( "%1) %2\n" ).arg( newFilms.count() + 1 ).arg( filmTitle ) );

            FilmItem* film = new FilmItem();
            film->SetColumnData( FilmItem::FileNameColumn, filmFileName );
            film->SetColumnData( FilmItem::TitleColumn, filmTitle );
            newFilms.append( film );
        }
    }

    if( newFilms.size() == 0 )
    {
        QMessageBox::information( this, tr("Add films"), tr("Nothing to add.") );
    }
    else
    {
        messageText = tr("Add the following film(s)?\n") + messageText;

        int answer = QMessageBox::question( this, tr("Add films"), messageText,
                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );

        if( answer == QMessageBox::Yes )
        {
            for( FilmItem* film : newFilms )
            {
                filmsListModel->AddFilmItem( film );
            }

            QMessageBox::information( this, tr("Add films"), tr("Done!") );
        }
        else
        {
            qDeleteAll( newFilms );
        }
    }

    DebugPrintFuncDone( "MainWindow::AddFilmsFromOutside" );
}


void MainWindow::closeEvent( QCloseEvent* event )
{
    hide();
    SaveSettings();
    event->accept();
}


void MainWindow::DatabaseConvertOld()
{
    QMessageBox::information( this,
                              Alexandra::appNameGui,
                              tr( "It seems you are running the new version of Alexandra for the old version of database. "
                                  "Backup your database in the next dialog before automatic conversion." ) );

    QFileInfo f( settings->GetDatabaseFilePath() );
    QString backupFileName = f.absolutePath() + "/" + f.baseName() + "_backup." + f.suffix();
    bool isDone = false;

    do
    {
        QString newName = QFileDialog::getSaveFileName( this, tr("Save backup"), backupFileName, tr("Alexandra DB (*.adat)") );

        if( newName.isEmpty() )
        {
            int answer = QMessageBox::warning( this,
                                               tr("Save backup"),
                                               tr( "Are you sure you don't want to save a backup? You can lose "
                                                   "your data if the conversion fails!" ),
                                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

            isDone = (answer == QMessageBox::Yes) ? true : false;
        }
        else
        {
            QFile::remove( newName );

            if( QFile::copy( f.absoluteFilePath(), newName ) )
            {
                isDone = true;
            }
            else
            {
                QMessageBox::warning( this, tr("Save backup"), tr("Unable to save file. Try to select another location.") );
            }
        }
    }
    while( !isDone );
}


void MainWindow::DatabaseChanged()
{
    statusbar->ShowTotal( filmsListModel->GetFilmsCount(),
                          filmsListModel->GetIsViewedFilmsCount(),
                          filmsListModel->GetIsFavouriteFilmsCount() );

    filmsListModel->SaveToFile( settings->GetDatabaseFilePath() );
}


void MainWindow::ResetStatistics()
{
    filmsListModel->ResetViews();
    QMessageBox::information( this, tr("Reset statistics"), tr("Done!") );
}


void MainWindow::ReloadDatabase()
{
    filmsListModel->LoadFromFile( settings->GetDatabaseFilePath() );
    SetCurrentFilmByTitle( settings->GetCurrentFilmTitle() );
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
    wFilmInfo->ShowMessage( tr("Error reading the database!") );
    QMessageBox::critical( this, tr("Database read error"), message );
}


void MainWindow::DatabaseIsEmpty()
{
    ClearTextFields();
    SetEmptyMode();
}


void MainWindow::DatabaseIsReadonly()
{
    SetReadOnlyMode();
    statusbar->SetAdditionalText( tr("The database file is read-only.") );
}


void MainWindow::ShowFilmInformation( const QModelIndex& index )
{
    if( index.isValid() )
    {
        const FilmItem* film = filmsListProxyModel->GetFilmItemByIndex( index );
        bool isExists = film->GetIsFileExists();

        bViewed->setEnabled( !filmsListModel->GetIsReadonly() );
        bViewed->setChecked( film->GetIsFilmViewed() );
        bFavourite->setEnabled( !filmsListModel->GetIsReadonly() );
        bFavourite->setChecked( film->GetIsFilmFavourite() );

        #ifdef MEDIAINFO_SUPPORT
            bTechInformation->setEnabled( isExists );
        #endif

        bPlay->setEnabled( isExists );
        bAddToPlaylist->setEnabled( isExists );
    }
    else
    {
        bViewed->setEnabled( false );
        bFavourite->setEnabled( false );
        bTechInformation->setEnabled( false );
        bPlay->setEnabled( false );
        bAddToPlaylist->setEnabled( false );
    }

    if( !wPlaylist->IsEmpty() )
    {
        bPlay->setEnabled( true );
    }

    if( filmsView->GetSelectedItemsList().count() > 1 )
    {
        bAddToPlaylist->setEnabled( true );
    }

      // Film info
    wFilmInfo->ShowInformation( index );
    lFilmPoster->ShowInformation( index );
    lTechInformation->ShowInformation( index );
}


void MainWindow::ShowFilmContextMenu( const QPoint& pos, const QModelIndex& index )
{
    const FilmItem* film = nullptr; // Few films selected

    if( filmsView->GetSelectedItemsList().count() == 1 )
    {
        film = filmsListProxyModel->GetFilmItemByIndex(index);
    }

    contextMenu->SetupMenuState( film );
    contextMenu->exec( dynamic_cast<QWidget*>( filmsView )->mapToGlobal( pos ) );
}


void MainWindow::QuickSearchSetFilterButton( int button, bool state )
{
    switch( button )
    {
        case SearchEdit::ShowViewed :
        {
            filmsListProxyModel->SetShowViewed( state );
            break;
        }
        case SearchEdit::ShowFavourite :
        {
            filmsListProxyModel->SetShowFavourite( state );
            break;
        }
        case SearchEdit::HideUnavailable :
        {
            filmsListProxyModel->SetHideUnavailable( state );
            break;
        }
    }

    statusbar->SetShown( filmsListProxyModel->rowCount() );
    QuickSearchCheckResult();
}


void MainWindow::QuickSearchSetFilter( const QString& text, QList<int> selectedColumns )
{
    filmsListProxyModel->SetFilter( text, selectedColumns );
    statusbar->SetShown( filmsListProxyModel->rowCount() );

    if( text.isEmpty() )
    {
        if( filmsView->GetCurrentIndex().isValid() )
        {
            filmsView->ScrollToCurrent();
        }
        else
        {
            filmsView->SetCurrentRow(0);
        }
    }
    else
    {
        QuickSearchCheckResult();
    }
}


void MainWindow::QuickSearchCheckResult()
{
    filmsView->SetCurrentRow(0);

    if( filmsListProxyModel->rowCount() == 0 )
    {
        lFilmPoster->Clear();
        wFilmInfo->ShowMessage( tr("Nothing was found"),
                                tr("Make sure all words are spelled correctly or try using other keywords.") );
    }
}


void MainWindow::QuickSearchEscape()
{
      // Pressing key 'Esc' sets focus to the films list,
      // pressing again clears the quick search line
    QAbstractItemView* view = dynamic_cast<QAbstractItemView*>( filmsView );

    if( view == focusWidget() )
    {
        eFilter->clear();
    }

    view->setFocus();
}


void MainWindow::AddToPlaylist()
{
    QModelIndexList indexes = filmsView->GetSelectedItemsList();

    if( indexes.count() > 0 )
    {
        for( const QModelIndex& index : indexes )
        {
            const FilmItem* film = filmsListProxyModel->GetFilmItemByIndex(index);

            if( film->GetIsFileExists() )
            {
                wPlaylist->AddItem( film->GetTitle(), film->GetFileName() );
            }
        }

        if( !wPlaylist->IsEmpty() )
        {
            bPlay->setText( tr("Play list") );
            bPlay->setEnabled( true );
        }
    }
}


void MainWindow::PlaylistCleared()
{
    bPlay->setText( tr("Play") );
    ShowFilmInformation( filmsView->GetCurrentIndex() );
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
        if( wPlaylist->IsEmpty() )
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
    if( bPlay->isEnabled() )
    {
        DebugPrintFunc( "MainWindow::PlayFilm" );

        if( externalPlayer->state() == QProcess::Running )
        {
            externalPlayer->close(); /// terminate (?)
        }
        else
        {
            QString playerPath = settings->GetExternalPlayer();
            QString fileToPlay;

            if( wPlaylist->IsEmpty() )
            {
                if( filmsView->GetSelectedItemsList().count() > 1)
                {
                    AddToPlaylist();
                    fileToPlay = PlayList( wPlaylist->GetPathes() ).CreateTempListM3U8();
                }
                else
                {
                    fileToPlay = filmsListProxyModel->GetFilmFileNameByIndex( filmsView->GetCurrentIndex() );
                }
            }
            else
            {
                fileToPlay = PlayList( wPlaylist->GetPathes() ).CreateTempListM3U8();
            }

            #ifdef Q_OS_LINUX
                externalPlayer->start( playerPath + " \"" + fileToPlay +"\"" );
            #elif defined(Q_OS_WIN32)
                externalPlayer->start( "\"" + playerPath + "\" \"" + fileToPlay +"\"" );
            #endif
        }

        DebugPrintFuncDone( "MainWindow::PlayFilm" );
    }
}


void MainWindow::PlayerStarted()
{
    DebugPrint( "Player started" );

    dynamic_cast<QWidget*>( filmsView )->setEnabled( false );
    wPlaylist->setEnabled( false );
    eFilter->setEnabled( false );
    bPlay->setText( tr("Stop") );
}


void MainWindow::PlayerClosed()
{
    DebugPrint( "Player stopped" );

    QWidget* view = dynamic_cast<QWidget*>( filmsView );
    view->setEnabled( true );
    view->setFocus();

    wPlaylist->setEnabled( true );
    eFilter->setEnabled( true );
    bPlay->setText( tr("Play") );

    if( wPlaylist->IsEmpty() )
    {
        QModelIndex index = filmsListProxyModel->mapToSource( filmsView->GetCurrentIndex() );
        filmsListModel->IncViewsCounterForIndex( index );
    }
    else
    {
        CheckViewedDialog list( wPlaylist->GetTitles(), this );
        QStringList viewedTitles = list.Exec();

        for( const QString& title : viewedTitles )
        {
            QModelIndex index = filmsListModel->GetFilmIndex( title );
            filmsListModel->IncViewsCounterForIndex( index );
            wPlaylist->RemoveItem( title );
        }
    }
}


void MainWindow::ShowSplashscreen()
{
    if( settings->GetApplicationShowSplashScreen() )
    {
        SplashScreen* splashScreen = new SplashScreen();
        splashScreen->show();
        qApp->processEvents();
    }
}


void MainWindow::ShowAddFilmWindow()
{
    AddFilmWindow* addFilmWindow = new AddFilmWindow( this );
    connect( addFilmWindow, &AddFilmWindow::Done, this, &MainWindow::AddFilmDone );
    addFilmWindow->show();
}


void MainWindow::ShowEditFilmWindow()
{
    QModelIndex index = filmsView->GetCurrentIndex();

    if( index.isValid() )
    {
        EditFilmWindow* editFilmWindow = new EditFilmWindow( this );
        connect( editFilmWindow, &EditFilmWindow::Done, this, &MainWindow::EditFilmDone );
        editFilmWindow->SetData( filmsListProxyModel->GetFilmItemByIndex(index) );
        editFilmWindow->show();
    }
}


void MainWindow::ShowRemoveFilmWindow()
{
    if( !filmsView->GetCurrentIndex().isValid() )
    {
        return;
    }

    QModelIndexList selectedIndexes = filmsView->GetSelectedItemsList();
    QString windowTitle = tr("Remove film");
    QString windowMessage;

    if( selectedIndexes.count() == 0 )
    {
        QMessageBox::warning( this, windowTitle, tr("Nothing was selected!") );
        return;
    }
    else if( selectedIndexes.count() == 1 ) // One film selected
    {
        QString filmTitle = filmsListProxyModel->GetFilmTitleByIndex( selectedIndexes.at(0) );
        windowMessage = tr("Are you sure you want to remove \"%1\"?").arg( filmTitle );
    }
    else // More than one film selected
    {
        auto AppendFilm = [this, selectedIndexes, &windowMessage] (int i)
        {
            QString filmTitle = filmsListProxyModel->GetFilmTitleByIndex( selectedIndexes.at(i) );
            windowMessage.append( QString( "%1) %2\n" ).arg( i + 1 ).arg( filmTitle ) );
        };

        windowMessage = tr("Are you sure you want to remove following films?\n");
        const int maxItemCount = 20;

        for( int i = 0; i < std::min( maxItemCount - 1, selectedIndexes.count() ); i++ )
        {
            AppendFilm( i );
        }

        if( selectedIndexes.count() == maxItemCount )
        {
            AppendFilm( maxItemCount - 1 );
        }
        else if( selectedIndexes.count() > maxItemCount )
        {
            windowMessage.append( "...\n" );
            AppendFilm( selectedIndexes.count() - 1 );
        }
    }

    int answer = QMessageBox::question( this, windowTitle, windowMessage );

    if( answer == QMessageBox::Yes )
    {
        DebugPrintFunc( "MainWindow::ShowRemoveFilmWindow" );
        QModelIndexList mappedIndexes;

        for( const QModelIndex& index : selectedIndexes )
        {
            mappedIndexes.append( filmsListProxyModel->mapToSource(index) );
        }

        for( const QModelIndex& index : mappedIndexes )
        {
            filmsListModel->RemoveFilmByIndex( index );
        }

        DebugPrintFuncDone( "MainWindow::ShowRemoveFilmWindow" );
    }
}


void MainWindow::ShowRemoveFileWindow()
{
    QModelIndex index = filmsView->GetCurrentIndex();
    const FilmItem* film = filmsListProxyModel->GetFilmItemByIndex( index );

    int answer = QMessageBox::question( this, tr("Remove file"),
                                        tr("Are you sure to remove file \"%1\"?").arg( film->GetTitle() ) );

    if( answer == QMessageBox::Yes )
    {
        QString filmFileName = film->GetFileName();

        if( QFile(filmFileName).remove() )
        {
            answer = QMessageBox::question( this, tr("Remove file"),
                                            tr("File was successfully removed. Do you want to remove record from the database?") );

            if( answer == QMessageBox::Yes )
            {
                filmsListModel->RemoveFilmByIndex( filmsListProxyModel->mapToSource(index) );
            }
            else
            {
                FilmItem* newFilm = new FilmItem( *film );
                newFilm->SetIsFileExists( FilmItem::NotExists );
                EditFilmDone( newFilm );
            }
        }
        else
        {
            QMessageBox::warning( this, tr("Remove file"), tr("Unable to remove file \"%1\"!").arg( filmFileName ) );
        }
    }
}


void MainWindow::ShowTechInfoWindow()
{
    #ifdef MEDIAINFO_SUPPORT
        FilmInfoWindow* filmInfoWindow = new FilmInfoWindow( this );
        filmInfoWindow->show();

        const FilmItem* film = filmsListProxyModel->GetFilmItemByIndex(filmsView->GetCurrentIndex()); /// get film from view (?)
        filmInfoWindow->LoadTechnicalInfoAsync( film->GetFileName() );
    #endif // MEDIAINFO_SUPPORT
}


void MainWindow::ShowFilmScannerWindow()
{
    QStringList fileNames;

    for( int row = 0; row < filmsListModel->GetFilmsCount(); ++row )
    {
        fileNames.append( filmsListModel->index( row, FilmItem::FileNameColumn ).data().toString() );
    }

    FilmScannerWindow* filmScannerWindow = new FilmScannerWindow( this );
    connect( filmScannerWindow, &FilmScannerWindow::AddFilm, this, &MainWindow::AddFilmDone );
    filmScannerWindow->SetExistedFileNames( fileNames );
    filmScannerWindow->show();
}


void MainWindow::ShowMovedFilmsWindow()
{
    FilmItemList films;

    for( int row = 0; row < filmsListModel->GetFilmsCount(); ++row )
    {
        FilmItem* film = static_cast<FilmItem*>( filmsListModel->index( row, 0 ).internalPointer() );

        if( !film->GetIsFileExists() && !film->GetFileName().isEmpty() )
        {
            films.append( film );
        }
    }

    if( films.isEmpty() )
    {
        QMessageBox::information( this, tr("Moved films"), tr("Nothing to move.") );
    }
    else
    {
        MovedFilmsWindow* movedFilmsWindow = new MovedFilmsWindow( this );
        connect( movedFilmsWindow, &MovedFilmsWindow::FilmsMoved, filmsListModel, &FilmsListModel::Invalidate );
        movedFilmsWindow->SetUnavailableFilms( films );
        movedFilmsWindow->show();
    }
}


void MainWindow::ShowStatisticsWindow()
{
    StatisticsWindow* statisticsWindow = new StatisticsWindow( this );
    connect( statisticsWindow, &StatisticsWindow::ResetStatistics, this, &MainWindow::ResetStatistics );
    statisticsWindow->SetModel( filmsListModel );
    statisticsWindow->show();
}


void MainWindow::ShowSearchWindow()
{
    SearchWindow* searchWindow = new SearchWindow( this );
    connect( searchWindow, &SearchWindow::ShowSelectedItem, this, &MainWindow::SetCurrentFilmByIndex );
    searchWindow->SetModel( filmsListModel );
    searchWindow->show();
}


void MainWindow::ShowSettingsWindow()
{
    SettingsWindow* settingsWindow = new SettingsWindow( this );
    connect( settingsWindow, &SettingsWindow::SettingsChanged,   this,           &MainWindow::ReloadSettings );
    connect( settingsWindow, &SettingsWindow::ViewChanged,       this,           &MainWindow::ReloadView );
    connect( settingsWindow, &SettingsWindow::DbSettingsChanged, this,           &MainWindow::ReloadDatabase );
    connect( settingsWindow, &SettingsWindow::EraseDatabase,     filmsListModel, &FilmsListModel::EraseAll );
    settingsWindow->show();
}


void MainWindow::SetCurrentFilmIsViewed( bool setViewed )
{
    QModelIndex index = filmsListProxyModel->mapToSource( filmsView->GetCurrentIndex() );

    if( setViewed )
    {
        filmsListModel->IncViewsCounterForIndex( index );
    }
    else // Uncheck
    {
        QMessageBox mb( this );
        mb.setWindowTitle( tr("Views counter") );
        mb.setText( tr("What do you want to do?") );
        QAbstractButton* cancelButton = mb.addButton( QMessageBox::Cancel );
        QAbstractButton* addViewButton = mb.addButton( tr("Add view"), QMessageBox::AcceptRole );
        QAbstractButton* resetViewsButton = mb.addButton( tr("Reset views"), QMessageBox::ResetRole);
        mb.setEscapeButton( cancelButton );

        mb.exec();

        if( mb.clickedButton() == addViewButton )
        {
            filmsListModel->IncViewsCounterForIndex( index );
        }
        else if( mb.clickedButton() == resetViewsButton )
        {
            filmsListModel->ResetViewsCounterForIndex( index );
        }
        else
        {
            bViewed->toggle();
        }
    }
}


void MainWindow::SelectRandomFilm()
{
    DebugPrintFunc( "MainWindow::SelectRandomFilm" );

    int count = filmsView->GetRowCount();

    if( count > 1 )
    {
        int currentRow = filmsView->GetCurrentIndex().row();
        int newRow;

        do { newRow = qrand() % count; }
        while( newRow == currentRow );

        filmsView->SetCurrentRow( newRow );
    }
    else
    {
        filmsView->SetCurrentRow( 0 );
    }

    DebugPrintFuncDone( "MainWindow::SelectRandomFilm" );
}


void MainWindow::AddFilmDone( FilmItem* film )
{
    filmsListModel->AddFilmItem( film );
    SetCurrentFilmByTitle( film->GetTitle() );
    SetAllFunctionsEnabled( true ); // If model was empty
}


void MainWindow::EditFilmDone( FilmItem* film )
{
    filmsListModel->EditFilmItem( film, filmsListProxyModel->mapToSource(filmsView->GetCurrentIndex()) );
}


void MainWindow::ToggleCurrentFilmValue( FilmItem::Column column )
{
    FilmItem* film = new FilmItem( *filmsListProxyModel->GetFilmItemByIndex(filmsView->GetCurrentIndex()) );
    film->SetColumnData( column, !film->GetColumnData( column ).toBool() );
    EditFilmDone( film );
}


void MainWindow::SetCurrentFilmByIndex( const QModelIndex& index )
{
    if( index.isValid() )
    {
        filmsView->SetCurrentIndex( filmsListProxyModel->mapFromSource(index) );
    }
    else
    {
        filmsView->SetCurrentRow( 0 );
    }
}


void MainWindow::SetCurrentFilmByTitle( const QString& title )
{
    QModelIndex index = filmsListModel->GetFilmIndex( title );
    SetCurrentFilmByIndex( index );
}


void MainWindow::SetupModels()
{
    DebugPrintFunc( "MainWindow::SetupModels" );

    filmsListModel = new FilmsListModel( this );
    connect( filmsListModel, &FilmsListModel::DatabaseConvertOld, this, &MainWindow::DatabaseConvertOld );

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

      // Quick search input
    eFilter->SetModel( filmsListProxyModel );
    connect( eFilter, &SearchEdit::ButtonFilterChanged, this, &MainWindow::QuickSearchSetFilterButton );
    connect( eFilter, &SearchEdit::FilterChanged, this, &MainWindow::QuickSearchSetFilter );

      // Playlist
    connect( contextMenu,    &FilmsViewContextMenu::actionAddToList, this, &MainWindow::AddToPlaylist );
    connect( bAddToPlaylist, &QPushButton::clicked,         this, &MainWindow::AddToPlaylist );
    connect( wPlaylist,      &PlaylistWidget::Cleared,      this, &MainWindow::PlaylistCleared );
    connect( wPlaylist,      &PlaylistWidget::ItemSelected, this, &MainWindow::SetCurrentFilmByTitle );

      // Viewed button
    connect( bViewed,     &QPushButton::clicked,                 this, &MainWindow::SetCurrentFilmIsViewed );
    connect( contextMenu, &FilmsViewContextMenu::actionIsViewed, this, &MainWindow::SetCurrentFilmIsViewed );
      // Favourite button
    connect( bFavourite,  &QPushButton::clicked,                    this, &MainWindow::SetCurrentFilmIsFavourite );
    connect( contextMenu, &FilmsViewContextMenu::actionIsFavourite, this, &MainWindow::SetCurrentFilmIsFavourite );
      // Play button
    connect( bPlay,       &QPushButton::clicked,             this, &MainWindow::PlayFilm );
    connect( contextMenu, &FilmsViewContextMenu::actionPlay, this, &MainWindow::PlayFilm );
      // Player setup
    connect( externalPlayer, &QProcess::started, this, &MainWindow::PlayerStarted );
    connect( externalPlayer, SIGNAL( finished(int) ), this, SLOT( PlayerClosed() ) );

        /// About window
    aboutWindow = new AboutWindow( this );
    connect( aboutWindow, &AboutWindow::UpdateAvailable, statusbar, &StatusBar::SetAdditionalText );
    connect( actionAbout,   &QAction::triggered, aboutWindow, &AboutWindow::show );
    connect( actionAboutQt, &QAction::triggered, aboutWindow, &AboutWindow::AboutQt );

        /// Add film window
    connect( actionAdd, &QAction::triggered, this, &MainWindow::ShowAddFilmWindow );
    connect( toolbar,   &ToolBar::actionAdd, this, &MainWindow::ShowAddFilmWindow );

        /// Edit film window
    connect( actionEdit,  &QAction::triggered,               this, &MainWindow::ShowEditFilmWindow );
    connect( toolbar,     &ToolBar::actionEdit,              this, &MainWindow::ShowEditFilmWindow );
    connect( contextMenu, &FilmsViewContextMenu::actionEdit, this, &MainWindow::ShowEditFilmWindow );

        /// Remove film window
    connect( actionRemove, &QAction::triggered,                 this, &MainWindow::ShowRemoveFilmWindow );
    connect( toolbar,      &ToolBar::actionRemove,              this, &MainWindow::ShowRemoveFilmWindow );
    connect( contextMenu,  &FilmsViewContextMenu::actionRemove, this, &MainWindow::ShowRemoveFilmWindow );

        /// Remove file
    connect( contextMenu, &FilmsViewContextMenu::actionRemoveFile, this, &MainWindow::ShowRemoveFileWindow );

        /// Random film function
    connect( actionRandom, &QAction::triggered,    this, &MainWindow::SelectRandomFilm );
    connect( toolbar,      &ToolBar::actionRandom, this, &MainWindow::SelectRandomFilm );

        /// Film info window
    #ifdef MEDIAINFO_SUPPORT
        connect( bTechInformation, &QPushButton::clicked,                 this, &MainWindow::ShowTechInfoWindow );
        connect( contextMenu,      &FilmsViewContextMenu::actionShowInfo, this, &MainWindow::ShowTechInfoWindow );
    #endif // MEDIAINFO_SUPPORT

        /// Search window
    connect( actionSearch, &QAction::triggered,    this, &MainWindow::ShowSearchWindow );
    connect( toolbar,      &ToolBar::actionSearch, this, &MainWindow::ShowSearchWindow );

        /// Settings window
    connect( actionSettings, &QAction::triggered, this, &MainWindow::ShowSettingsWindow );

        /// Film scanner window
    connect( actionFilmScanner, &QAction::triggered,         this, &MainWindow::ShowFilmScannerWindow );
    connect( toolbar,           &ToolBar::actionFilmScanner, this, &MainWindow::ShowFilmScannerWindow );

        /// Moved films window
    connect( actionMovedFilms, &QAction::triggered, this, &MainWindow::ShowMovedFilmsWindow );

        /// Statistic window
    connect( actionStatistics, &QAction::triggered, this, &MainWindow::ShowStatisticsWindow );

        /// Shortcuts
    quickSearchShortcut = new QShortcut( this );
    quickSearchShortcut->setContext( Qt::ApplicationShortcut );
    connect( quickSearchShortcut, SIGNAL(activated()), eFilter, SLOT(setFocus()) );

    viewFocusShortcut = new QShortcut( this );
    viewFocusShortcut->setKey( QKeySequence( "Esc" ) );
    viewFocusShortcut->setContext( Qt::ApplicationShortcut );
    connect( viewFocusShortcut, &QShortcut::activated, this, &MainWindow::QuickSearchEscape );

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
    connect( view, SIGNAL(CurrentChanged(QModelIndex)),   this, SLOT(ShowFilmInformation(QModelIndex)) );
    connect( view, SIGNAL(CurrentActivated(QModelIndex)), this, SLOT(DoubleClickBehavior()) );
    connect( view, SIGNAL(ContextMenuRequested(QPoint,QModelIndex)), this, SLOT(ShowFilmContextMenu(QPoint,QModelIndex)) );

    view->setModel( filmsListProxyModel );
    vlLeft->insertWidget( 0, view );
    view->show();
    view->setFocus();

    DebugPrintFuncDone( "MainWindow::SetupFilmsView" );
}


void MainWindow::LoadSettings()
{
    DebugPrintFunc( "MainWindow::LoadSettings" );

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
    wPlaylist->LoadSettings();

    if( !wPlaylist->IsEmpty() )
    {
        bPlay->setText( tr("Play list") );
    }
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

    if( style == tr("<Theme>") )
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
    wPlaylist->SaveSettings();

    settings->SetCurrentFilmTitle( filmsListProxyModel->GetFilmTitleByIndex( filmsView->GetCurrentIndex() ) );
    settings->sync();

    DebugPrintFuncDone( "MainWindow::SaveSettings" );
}


void MainWindow::ClearTextFields()
{
    wFilmInfo->Clear();
    lFilmPoster->Clear();
    lTechInformation->clear();
}


void MainWindow::SetAllFunctionsEnabled( bool enabled )
{
    toolbar->SetAllFunctionsEnabled( enabled );

    actionAdd->setEnabled( enabled );
    actionEdit->setEnabled( enabled );
    actionRemove->setEnabled( enabled );
    actionRandom->setEnabled( enabled );
    actionSearch->setEnabled( enabled );

    actionFilmScanner->setEnabled( enabled );
    actionMovedFilms->setEnabled( enabled );
    actionStatistics->setEnabled( enabled );

    bViewed->setEnabled( enabled );
    bFavourite->setEnabled( enabled );
    eFilter->setEnabled( enabled );
}


void MainWindow::SetEmptyMode( bool empty )
{
    toolbar->SetEmptyDatabaseMode( empty );

    actionEdit->setDisabled( empty );
    actionRemove->setDisabled( empty );
    actionRandom->setDisabled( empty );
    actionSearch->setDisabled( empty );

    actionMovedFilms->setDisabled( empty );
    actionStatistics->setDisabled( empty );

    bViewed->setDisabled( empty );
    bFavourite->setDisabled( empty );
    bTechInformation->setDisabled( empty );
    bPlay->setDisabled( empty );
    bAddToPlaylist->setDisabled( empty );

    eFilter->setDisabled( empty );
    wFilmInfo->ShowEmptyDatabaseMessage();
    lFilmPoster->Clear();
    lTechInformation->Clear();
}


void MainWindow::SetReadOnlyMode( bool readOnly )
{
    toolbar->SetReadOnlyMode( readOnly );
    contextMenu->SetReadOnlyMode( readOnly );

    actionAdd->setDisabled( readOnly );
    actionEdit->setDisabled( readOnly );
    actionRemove->setDisabled( readOnly );

    actionFilmScanner->setDisabled( readOnly );
    actionMovedFilms->setDisabled( readOnly );
    actionStatistics->setDisabled( readOnly );

    bViewed->setDisabled( readOnly );
    bFavourite->setDisabled( readOnly );
}

