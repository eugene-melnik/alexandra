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
#include <QSettings>
#include <QString>

class AlexandraSettings : public QSettings
{
    public:
        AlexandraSettings( QObject* parent = nullptr );

        // Get //

        QString GetDatabaseFilePath() const;
        QString GetExternalPlayer() const;
        QString GetLastFilmPath() const;
        QString GetLastPosterPath() const;
        int     GetApplicationLocaleIndex() const;
        QString GetApplicationStyleName() const;

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
        int     GetScalePosterToHeight() const;
        QRgb    GetUnavailableFileColor() const;
        int     GetListFontSize() const;
        int     GetListRowHeight() const;

        int GetGridItemSize() const;
        int GetGridFontSize() const;
        bool GetGridShowTooltip() const;

        QByteArray GetMainWindowGeometry() const;
        QByteArray GetMainWindowState() const;
        QByteArray GetMainWindowSplitterState() const;
        int        GetMainWindowToolbarStyle() const;
        bool       GetMainWindowShowRightPanel() const;

        // Set //

        void SetDatabaseFilePath( const QString& s );
        void SetExternalPlayer( const QString& s );
        void SetLastFilmPath( const QString& s );
        void SetLastPosterPath( const QString& s );
        void SetApplicationLocaleIndex( int n );
        void SetApplicationStyleName( const QString& s );

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
};

#endif // ALEXANDRASETTINGS_H
