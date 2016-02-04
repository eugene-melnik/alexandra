/*************************************************************************************************
 *                                                                                                *
 *  file: mainwindow.h                                                                            *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filmslist/filmslistmodel.h"
#include "filmslist/filmslistproxymodel.h"
#include "filmsview/abstractfilmsview.h"
#include "filmsview/filmsviewcontextmenu.h"
//#include "scanner/filmscannerwindow.h"
//#include "scanner/movedfilmswindow.h"
#include "search/searchedit.h"
//#include "search/searchwindow.h"
#include "aboutwindow.h"
#include "alexandrasettings.h"
//#include "filminfowindow.h"
#include "settingswindow.h"
//#include "statisticswindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QPoint>
#include <QProcess>
#include <QShortcut>
#include <QSortFilterProxyModel>

class MainWindow : public QMainWindow, protected Ui::MainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow();
        virtual ~MainWindow() = default;

        void AddFilmsFromOutside( const QStringList& films );

    signals:
        void Shown();

    protected:
        void closeEvent( QCloseEvent* event ) override;

    private slots:
        void ShowFullScreen( bool isFullScreen ) { isFullScreen ? showFullScreen() : showNormal(); }
        void QuickSearchEscBehavior();

        void DatabaseChanged();
//        void ResetStatistics();
        void ReloadDatabase();
        void ReloadSettings();
        void ReloadView();

        void DatabaseIsLoaded() { SetAllFunctionsEnabled( true ); }
        void DatabaseReadError( const QString& message );
        void DatabaseIsEmpty();
        void DatabaseIsReadonly();

        void ShowFilmInformation( const QModelIndex& index );
        void ShowFilmContextMenu( const QPoint& pos, const QModelIndex& index );

//        void AddToPlaylist();
        void PlaylistCleared();
        void DoubleClickBehavior();

//        void PlayFilm();
        void PlayerStarted();
//        void PlayerClosed();

//        void RemoveFilm();
//        void RemoveFile();
//        void FilmScanner() { filmScannerWindow->show( filmsList->GetFileNamesList() ); }
//        void MovedFilms() { movedFilmsWindow->show( filmsList->GetUnavailablesList() ); }
//        void Statistics() { statisticsWindow->show( filmsList->GetFilmsList() ); }

//        void UpdateCurrentFilm() { UpdateCurrentFilm( Film() ); }
//        void UpdateCurrentFilm( Film film );

        void ShowAddFilmWindow();
        void ShowEditFilmWindow();

    private:
        void AddFilmDone( FilmItem* film );
        void EditFilmDone( FilmItem* film );
//        void AddFilmsDone( const QList<Film>* films );

        void ShowSplashscreen();
        void SetupModels();
        void SetupWindows();
        void SetupFilmsView();

        void LoadSettings();
        void LoadAppearance();
        void LoadShorcuts();
        void SaveSettings();

        void ClearTextFields();
        void SetAllFunctionsEnabled( bool b );
        void SetEmptyMode( bool b = true );
        void SetReadOnlyMode( bool b = true );

        // Child windows
        AboutWindow* aboutWindow;
//        FilmInfoWindow* filmInfoWindow = nullptr;
//        FilmScannerWindow* filmScannerWindow = nullptr;
//        MovedFilmsWindow* movedFilmsWindow = nullptr;
//        SearchWindow* searchWindow = nullptr;
        SettingsWindow* settingsWindow = nullptr;
//        StatisticsWindow* statisticsWindow = nullptr;

        // Shortcuts
        QShortcut* quickSearchShortcut;
        QShortcut* viewFocusShortcut;

        // Variables
        AbstractFilmsView* filmsView = nullptr;
        FilmsViewContextMenu* contextMenu;

        AlexandraSettings* settings;
        QProcess* externalPlayer;
        FilmsListModel* filmsListModel;
        FilmsListProxyModel* filmsListProxyModel;
};

#endif // MAINWINDOW_H
