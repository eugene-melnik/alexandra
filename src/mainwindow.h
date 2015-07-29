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

#include "aboutwindow.h"
#include "abstractfilmsview.h"
#include "addfilmwindow.h"
#include "alexandrasettings.h"
#include "editfilmwindow.h"
#include "filminfowindow.h"
#include "filmslist.h"
#include "filmscannerwindow.h"
#include "filmsviewcontextmenu.h"
#include "movedfilmswindow.h"
#include "searchwindow.h"
#include "settingswindow.h"
#include "splashscreen.h"
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
        MainWindow( QWidget* parent = nullptr );
        ~MainWindow();

    signals:
        void Shown();

    protected:
        void closeEvent( QCloseEvent* event );

    private slots:
        void ShowFullScreen( bool isFullScreen );

        void SaveDatabase();
        void EraseDatabase();
        void ReloadDatabase();
        void ReloadSettings();
        void ReloadView();

        void DatabaseIsLoaded();
        void DatabaseReadError();
        void DatabaseIsEmpty();
        void DatabaseIsReadonly();

        void ShowFilms();
        void ShowFilmInformation();
        void ShowFilmContextMenu( QPoint p );
        void ShowShortTechnicalInfo( QString info );

        void PlayFilm();
        void PlayerStarted();
        void PlayerClosed();

        void EditFilm();
        void RemoveFilm();
        void RemoveFile();
        void FilmScanner();
        void MovedFilms();

        void SetupCompleter();
        void FilmsFilter( QString key );

    private:
        // Additional functions
        void LoadSettings();
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

        // Variables
        AbstractFilmsView* filmsView = nullptr;
        FilmsViewContextMenu* contextMenu = nullptr;

        AlexandraSettings* settings = nullptr;
        FilmsList* filmsList = nullptr;
        QProcess* externalPlayer = nullptr;
};

#endif // MAINWINDOW_H
