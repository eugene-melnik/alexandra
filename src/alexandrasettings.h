/*************************************************************************************************
 *                                                                                                *
 *  file: alexandrasettings.h                                                                     *
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

#ifndef ALEXANDRASETTINGS_H
#define ALEXANDRASETTINGS_H

#include <QByteArray>
#include <QColor>
#include <QFont>
#include <QSettings>
#include <QString>

class AlexandraSettings : public QSettings
{
    public:
        static void Initialize( const QString& configFile = QString() );
        static AlexandraSettings* GetInstance();

        /* Get */

        QString GetDatabaseFilePath() const;
        QString GetExternalPlayer() const;
        QString GetLastFilmPath() const;
        QString GetLastPosterPath() const;
        int     GetApplicationLocaleIndex() const;
        QString GetApplicationFont() const;
        QString GetApplicationStyleName() const;
        int     GetApplicationThemeIndex() const;
        bool    GetApplicationShowSplashScreen() const;

        int     GetFilmsViewMode() const;

        bool    GetCheckFilesOnStartup() const;
        int     GetColumnViewedWidth() const;
        int     GetColumnFavouriteWidth() const;
        int     GetColumnTitleWidth() const;
        int     GetColumnYearWidth() const;
        int     GetColumnGenreWidth() const;
        int     GetColumnDirectorWidth() const;
        QString GetCurrentFilmTitle() const;
        QString GetPostersDirPath() const;
        QString GetPosterSavingFormat() const;
        int     GetPosterSavingQuality() const;
        int     GetScalePosterToHeight() const;
        QRgb    GetUnavailableFileColor() const;
        int     GetListFontSize() const;
        int     GetListRowHeight() const;

        int  GetGridItemSize() const;
        int  GetGridFontSize() const;
        bool GetGridShowTooltip() const;

        QByteArray GetMainWindowGeometry() const;
        QByteArray GetMainWindowState() const;
        QByteArray GetMainWindowSplitterState() const;
        int        GetMainWindowToolbarStyle() const;
        bool       GetMainWindowShowRightPanel() const;
        int        GetMainWindowRightPanelWidth() const;
        bool       GetMainWindowStatusbarIsVisible() const;
        int        GetQuickSearchFilter() const;
        bool       GetAutoLoadTechInfo() const;

        QString GetFilmsScannerLastDir() const;
        int     GetDefaultParserIndex() const;

        bool GetParsersKinoteatrLoadAdvancedInfo() const;
        bool GetParsersKinoteatrLoadBigPoster() const;

        QString GetShortcutAddFilm() const;
        QString GetShortcutEditFilm() const;
        QString GetShortcutRemoveFilm() const;
        QString GetShortcutSelectRandomFilm() const;
        QString GetShortcutActivateQuickSearch() const;
        QString GetShortcutAdvancedSearch() const;
        QString GetShortcutPlay() const;
        QString GetShortcutSettings() const;
        QString GetShortcutShowToolbar() const;
        QString GetShortcutShowFullscreen() const;
        QString GetShortcutExit() const;

        QString GetPlayerDoubleClickBehavior() const;

        /* Set */

        void SetDatabaseFilePath( const QString& s );
        void SetExternalPlayer( const QString& s );
        void SetLastFilmPath( const QString& s );
        void SetLastPosterPath( const QString& s );
        void SetApplicationLocaleIndex( int n );
        void SetApplicationFont( const QString& s );
        void SetApplicationStyleName( const QString& s );
        void SetApplicationThemeIndex( int n );
        void SetApplicationShowSplashScreen( bool b );

        void SetFilmsViewMode( int n );

        void SetCheckFilesOnStartup( bool b );
        void SetColumnViewedWidth( int n );
        void SetColumnFavouriteWidth( int n );
        void SetColumnTitleWidth( int n );
        void SetColumnYearWidth( int n );
        void SetColumnGenreWidth( int n );
        void SetColumnDirectorWidth( int n );
        void SetCurrentFilmTitle( const QString& s );
        void SetPostersDirPath( const QString& s );
        void SetPosterSavingFormat( const QString& s );
        void SetPosterSavingQuality( int n );
        void SetScalePostersToHeight( int n );
        void SetUnavailableFileColor( QRgb c );
        void SetListFontSize( int n );
        void SetListRowHeight( int n );

        void SetGridItemSize( int n );
        void SetGridFontSize( int n );
        void SetGridShowTooltip( bool b );

        void SetMainWindowGeometry( const QByteArray& a );
        void SetMainWindowState( const QByteArray& a );
        void SetMainWindowSplitterState( const QByteArray& a );
        void SetMainWindowToolbarStyle( int n );
        void SetMainWindowShowRightPanel( bool b );
        void SetMainWindowRightPanelWidth( int n );
        void SetMainWindowStatusbarIsVisible( bool b );
        void SetQuickSearchFilter( int n );
        void SetAutoLoadTechInfo( bool b );

        void SetFilmsScannerLastDir( const QString& s );
        void SetDefaultParserIndex( int n );

        void SetShortcutAddFilm( const QString& s );
        void SetShortcutEditFilm( const QString& s );
        void SetShortcutRemoveFilm( const QString& s );
        void SetShortcutSelectRandomFilm( const QString& s );
        void SetShortcutActivateQuickSearch( const QString& s );
        void SetShortcutAdvancedSearch( const QString& s );
        void SetShortcutPlay( const QString& s );
        void SetShortcutSettings( const QString& s );
        void SetShortcutShowToolbar( const QString& s );
        void SetShortcutShowFullscreen( const QString& s );
        void SetShortcutExit( const QString& s );

        void SetPlayerDoubleClickBehavior( const QString& s );

    private:
        AlexandraSettings( const QString& configFile );
        AlexandraSettings( const AlexandraSettings& s ) = delete;

        static AlexandraSettings* instance;
};

#endif // ALEXANDRASETTINGS_H
