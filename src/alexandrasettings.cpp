/*************************************************************************************************
 *                                                                                                *
 *  file: alexandrasettings.cpp                                                                   *
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

#include "alexandrasettings.h"
#include "version.h"

#include <QFileInfo>
#include <QProcessEnvironment>

AlexandraSettings::AlexandraSettings( QObject* parent )
    : QSettings( Alexandra::appName, "configuration", parent )
{
    // Set database filename

    QString databaseFileName = GetApplicationDatabaseFile();

    if( databaseFileName.isEmpty() )
    {
#ifdef Q_OS_LINUX
        databaseFileName = QProcessEnvironment::systemEnvironment().value( "XDG_CONFIG_HOME" );

        if( databaseFileName.isEmpty() ) {
            databaseFileName = QProcessEnvironment::systemEnvironment().value( "HOME" ) + "/.config";
        }
#elif defined( Q_OS_WIN32 )
        databaseFileName = QProcessEnvironment::systemEnvironment().value( "APPDATA" );
#endif
        databaseFileName += "/" + Alexandra::appName + "/database.adat";

        SetApplicationDatabaseFile( databaseFileName );
    }

    // Set posters' directory name

    QString postersFolderName = GetFilmsListPostersDir();
    QString databaseDir = QFileInfo( databaseFileName ).absolutePath();

    if( postersFolderName.isEmpty() )
    {
        postersFolderName = databaseDir + "/posters";
        SetFilmsListPostersDir( postersFolderName );
    }

    // Set external player

    QString externalPlayerName = GetApplicationExternalPlayer();

    if( externalPlayerName.isEmpty() )
    {
#ifdef Q_OS_LINUX
        externalPlayerName = "xdg-open";
#elif defined(Q_OS_WIN32)
        externalPlayerName = "C:\\Program Files\\Windows Media Player\\wmplayer.exe";
#else
        externalPlayerName.clear();
#endif
        SetApplicationExternalPlayer( externalPlayerName );
    }

    sync();
}

// Get //

QString AlexandraSettings::GetApplicationDatabaseFile() const
{
    return( value( "Application/DatabaseFile", "" ).toString() );
}

QString AlexandraSettings::GetApplicationExternalPlayer() const
{
    return( value( "Application/ExternalPlayer", "" ).toString() );
}

QString AlexandraSettings::GetApplicationLastFilmPath() const
{
    return( value( "Application/LastFilmPath", "." ).toString() );
}

QString AlexandraSettings::GetApplicationLastPosterPath() const
{
    return( value( "Application/LastPosterPath", "." ).toString() );
}

int AlexandraSettings::GetApplicationLocaleIndex() const
{
    return( value( "Application/Locale", -1 ).toInt() );
}

QString AlexandraSettings::GetApplicationStyle() const
{
    return( value( "Application/Style" ).toString() );
}

bool AlexandraSettings::GetFilmsListCheckFilesOnStartup() const
{
    return( value( "FilmsList/CheckFilesOnStartup", true ).toBool() );
}

int AlexandraSettings::GetFilmsListColumnViewedW() const
{
    int defaultValue = 20;
    return( value( "FilmsList/ColumnViewedW", defaultValue ).toInt() );
}

int AlexandraSettings::GetFilmsListColumnFavouriteW() const
{
    int defaultValue = 20;
    return( value( "FilmsList/ColumnFavouriteW", defaultValue ).toInt() );
}

int AlexandraSettings::GetFilmsListColumnTitleW() const
{
    int defaultValue = 150;
    return( value( "FilmsList/ColumnTitleW", defaultValue ).toInt() );
}

int AlexandraSettings::GetFilmsListColumnYearW() const
{
    int defaultValue = 50;
    return( value( "FilmsList/ColumnYearW", defaultValue ).toInt() );
}

int AlexandraSettings::GetFilmsListColumnGenreW() const
{
    int defaultValue = 110;
    return( value( "FilmsList/ColumnGenreW", defaultValue ).toInt() );
}

int AlexandraSettings::GetFilmsListColumnDirectorW() const
{
    int defaultValue = 110;
    return( value( "FilmsList/ColumnDirectorW", defaultValue ).toInt() );
}

QString AlexandraSettings::GetFilmsListCurrentFilm() const
{
    return( value( "FilmsList/CurrentFilm", "" ).toString() );
}

QString AlexandraSettings::GetFilmsListPostersDir() const
{
    return( value( "FilmsList/PostersDir", "" ).toString() );
}

int AlexandraSettings::GetFilmsListScalePosters() const
{
    return( value( "FilmsList/ScalePosters", 600 ).toInt() );
}

QRgb AlexandraSettings::GetFilmsListUnavailableFileColor() const
{
    return( value( "FilmsList/UnavailableFileColor", qRgb( 0xff, 0xc0, 0xc0 ) ).toUInt() );
}

QByteArray AlexandraSettings::GetMainWindowGeometry() const
{
    return( value( "MainWindow/Geometry" ).toByteArray() );
}

QByteArray AlexandraSettings::GetMainWindowState() const
{
    return( value( "MainWindow/State" ).toByteArray() );
}

int AlexandraSettings::GetMainWindowToolbarStyle() const
{
    return( value( "MainWindow/ToolbarStyle", Qt::ToolButtonFollowStyle ).toInt() );
}

// Set //

void AlexandraSettings::SetApplicationDatabaseFile( const QString& s )
{
    setValue( "Application/DatabaseFile", s );
}

void AlexandraSettings::SetApplicationExternalPlayer( const QString& s )
{
    setValue( "Application/ExternalPlayer", s );
}

void AlexandraSettings::SetApplicationLastFilmPath( const QString& s )
{
    setValue( "Application/LastFilmPath", s );
}

void AlexandraSettings::SetApplicationLastPosterPath( const QString& s )
{
    setValue( "Application/LastPosterPath", s );
}

void AlexandraSettings::SetApplicationLocaleIndex( int n )
{
    setValue( "Application/Locale", n );
}

void AlexandraSettings::SetApplicationStyle( const QString& s )
{
    setValue( "Application/Style", s );
}

void AlexandraSettings::SetFilmsListCheckFilesOnStartup( bool b )
{
    setValue( "FilmsList/CheckFilesOnStartup", b );
}

void AlexandraSettings::SetFilmsListColumnViewedW( int n )
{
    setValue( "FilmsList/ColumnViewedW", n );
}

void AlexandraSettings::SetFilmsListColumnFavouriteW( int n )
{
    setValue( "FilmsList/ColumnFavouriteW", n );
}

void AlexandraSettings::SetFilmsListColumnTitleW( int n )
{
    setValue( "FilmsList/ColumnTitleW", n );
}

void AlexandraSettings::SetFilmsListColumnYearW( int n )
{
    setValue( "FilmsList/ColumnYearW", n );
}

void AlexandraSettings::SetFilmsListColumnGenreW( int n )
{
    setValue( "FilmsList/ColumnGenreW", n );
}

void AlexandraSettings::SetFilmsListColumnDirectorW( int n )
{
    setValue( "FilmsList/ColumnDirectorW", n );
}

void AlexandraSettings::SetFilmsListCurrentFilm( const QString& s )
{
    setValue( "FilmsList/CurrentFilm", s );
}

void AlexandraSettings::SetFilmsListPostersDir( const QString& s )
{
    setValue( "FilmsList/PostersDir", s );
}

void AlexandraSettings::SetFilmsListScalePosters( int n )
{
    setValue( "FilmsList/ScalePosters", n );
}

void AlexandraSettings::SetFilmsListUnavailableFileColor( QRgb c )
{
    setValue( "FilmsList/UnavailableFileColor", c );
}

void AlexandraSettings::SetMainWindowGeometry( const QByteArray& a )
{
    setValue( "MainWindow/Geometry", a );
}

void AlexandraSettings::SetMainWindowState( const QByteArray& a )
{
    setValue( "MainWindow/State", a );
}

void AlexandraSettings::SetMainWindowToolbarStyle( int n )
{
    setValue( "MainWindow/ToolbarStyle", n );
}
