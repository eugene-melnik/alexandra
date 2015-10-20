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

#include "filmslist/filmslist.h"
#include "filmsview/abstractfilmsview.h"
#include "filmsview/filmsviewcontextmenu.h"
#include "scanner/filmscannerwindow.h"
#include "scanner/movedfilmswindow.h"
#include "search/searchedit.h"
#include "search/searchwindow.h"
#include "aboutwindow.h"
#include "addfilmwindow.h"
#include "alexandrasettings.h"
#include "editfilmwindow.h"
#include "filminfowindow.h"
#include "settingswindow.h"
#include "splashscreen.h"
#include "statisticswindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMainWindow>
#include <QPoint>
#include <QProcess>
#include <QString>

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow();
        virtual ~MainWindow() = default;

        void AddFilmsFromOutside( const QStringList& films );

    signals:
        void Shown();

    protected:
        void closeEvent( QCloseEvent* event );

    private slots:
        void ShowFullScreen( bool isFullScreen ) { isFullScreen ? showFullScreen() : showNormal(); }
        void QuickSearchEscBehavior();

        void SaveDatabase();
        void EraseDatabase();
        void ResetStatistics();
        void ReloadDatabase();
        void ReloadSettings();
        void ReloadView();

        void DatabaseIsLoaded() { SetAllFunctionsEnabled( true ); }
        void DatabaseReadError();
        void DatabaseIsEmpty();
        void DatabaseIsReadonly();

        void ShowFilms();
        void ShowFilmInformation();
        void ShowFilmContextMenu( QPoint p );
        void ShowShortTechnicalInfo( const QString& info );

        void AddToPlaylist();
        void PlaylistCleared();
        void DoubleClickBehavior();

        void PlayFilm();
        void PlayerStarted();
        void PlayerClosed();

        void EditFilm() { editFilmWindow->show( filmsList->GetCurrentFilm() ); }
        void RemoveFilm();
        void RemoveFile();
        void FilmScanner() { filmScannerWindow->show( filmsList->GetFileNamesList() ); }
        void MovedFilms() { movedFilmsWindow->show( filmsList->GetUnavailablesList() ); }
        void Statistics() { statisticsWindow->show( filmsList->GetFilmsList() ); }

        void SetupCompleter();
        void FilmsFilter( const QString& key, SearchEdit::FilterBy filters );
        void UpdateCurrentFilm() { UpdateCurrentFilm( Film() ); }
        void UpdateCurrentFilm( Film film );

        void SetProgressMaximum( int n );
        void SetProgressValue( int n );
        void StatusbarShowTotal();

    private:
        // Additional functions
        void AddFilmDone( Film film );
        void AddFilmsDone( const QList<Film>* films );

        void LoadSettings();
        void LoadAppearance();
        void LoadShorcuts();
        void SaveSettings();

        void SetupFilmsView();
        void SetupWindows();

        void ClearTextFields();
        void SetAllFunctionsEnabled( bool b );
        void SetEmptyMode( bool b = true );
        void SetReadOnlyMode( bool b = true );

        // Child windows
        AboutWindow* aboutWindow = nullptr;
        AddFilmWindow* addFilmWindow = nullptr;
        EditFilmWindow* editFilmWindow = nullptr;
        FilmInfoWindow* filmInfoWindow = nullptr;
        FilmScannerWindow* filmScannerWindow = nullptr;
        MovedFilmsWindow* movedFilmsWindow = nullptr;
        SearchWindow* searchWindow = nullptr;
        SettingsWindow* settingsWindow = nullptr;
        SplashScreen* splashScreen = nullptr;
        StatisticsWindow* statisticsWindow = nullptr;

        // Variables
        AbstractFilmsView* filmsView = nullptr;
        FilmsViewContextMenu* contextMenu = nullptr;

        AlexandraSettings* settings = nullptr;
        FilmsList* filmsList = nullptr;
        QProcess* externalPlayer = nullptr;
};

#endif // MAINWINDOW_H
