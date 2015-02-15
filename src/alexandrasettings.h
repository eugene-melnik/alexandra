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
        AlexandraSettings( const QString& organization,
                           const QString& application = QString(),
                           QObject* parent = nullptr );

        // Get //

        QString GetApplicationDatabaseFile() const;
        QString GetApplicationExternalPlayer() const;
        QString GetApplicationLastFilmPath() const;
        QString GetApplicationLastPosterPath() const;
        QString GetApplicationStyle() const;

        bool    GetFilmsListCheckFilesOnStartup() const;
        int     GetFilmsListColumnViewedW() const;
        int     GetFilmsListColumnFavouriteW() const;
        int     GetFilmsListColumnTitleW() const;
        int     GetFilmsListColumnYearW() const;
        int     GetFilmsListColumnGenreW() const;
        int     GetFilmsListColumnDirectorW() const;
        QString GetFilmsListCurrentFilm() const;
        QString GetFilmsListPostersDir() const;
        int     GetFilmsListScalePosters() const;
        QRgb    GetFilmsListUnavailableFileColor() const;

        QByteArray GetMainWindowGeometry() const;
        QByteArray GetMainWindowState() const;
        int        GetMainWindowToolbarStyle() const;

        // Set //

        void SetApplicationDatabaseFile( const QString& s );
        void SetApplicationExternalPlayer( const QString& s );
        void SetApplicationLastFilmPath( const QString& s );
        void SetApplicationLastPosterPath( const QString& s );
        void SetApplicationStyle( const QString& s );

        void SetFilmsListCheckFilesOnStartup( bool b );
        void SetFilmsListColumnViewedW( int n );
        void SetFilmsListColumnFavouriteW( int n );
        void SetFilmsListColumnTitleW( int n );
        void SetFilmsListColumnYearW( int n );
        void SetFilmsListColumnGenreW( int n );
        void SetFilmsListColumnDirectorW( int n );
        void SetFilmsListCurrentFilm( const QString& s );
        void SetFilmsListPostersDir( const QString& s );
        void SetFilmsListScalePosters( int n );
        void SetFilmsListUnavailableFileColor( QRgb c );

        void SetMainWindowGeometry( const QByteArray& a );
        void SetMainWindowState( const QByteArray& a );
        void SetMainWindowToolbarStyle( int n );
};

#endif // ALEXANDRASETTINGS_H
