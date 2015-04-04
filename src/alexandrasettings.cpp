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

    QString databaseFileName = GetDatabaseFilePath();

    if( databaseFileName.isEmpty() )
    {
#ifdef Q_OS_LINUX
        databaseFileName = QProcessEnvironment::systemEnvironment().value( "XDG_CONFIG_HOME" );

        if( databaseFileName.isEmpty() )
        {
            databaseFileName = QProcessEnvironment::systemEnvironment().value( "HOME" ) + "/.config";
        }
#elif defined( Q_OS_WIN32 )
        databaseFileName = QProcessEnvironment::systemEnvironment().value( "APPDATA" );
#endif
        databaseFileName += "/" + Alexandra::appName + "/database.adat";

        SetDatabaseFilePath( databaseFileName );
    }

    // Set posters' directory name

    QString postersFolderName = GetPostersDirPath();
    QString databaseDir = QFileInfo( databaseFileName ).absolutePath();

    if( postersFolderName.isEmpty() )
    {
        postersFolderName = databaseDir + "/posters";
        SetPostersDirPath( postersFolderName );
    }

    // Set external player

    QString externalPlayerName = GetExternalPlayer();

    if( externalPlayerName.isEmpty() )
    {
#ifdef Q_OS_LINUX
        externalPlayerName = "xdg-open";
#elif defined(Q_OS_WIN32)
        externalPlayerName = "C:\\Program Files\\Windows Media Player\\wmplayer.exe";
#endif
        SetExternalPlayer( externalPlayerName );
    }

    sync();
}

// Get //

QString AlexandraSettings::GetDatabaseFilePath() const
{
    return( value( "Application/DatabaseFile", "" ).toString() );
}

QString AlexandraSettings::GetExternalPlayer() const
{
    return( value( "Application/ExternalPlayer", "" ).toString() );
}

QString AlexandraSettings::GetLastFilmPath() const
{
    return( value( "Application/LastFilmPath", "." ).toString() );
}

QString AlexandraSettings::GetLastPosterPath() const
{
    return( value( "Application/LastPosterPath", "." ).toString() );
}

int AlexandraSettings::GetApplicationLocaleIndex() const
{
    return( value( "Application/Locale", -1 ).toInt() );
}

QString AlexandraSettings::GetApplicationStyleName() const
{
    return( value( "Application/Style" ).toString() );
}

int AlexandraSettings::GetFilmsViewMode() const
{
    return( value( "FilmsView/Mode", 0 ).toInt() );
}

bool AlexandraSettings::GetCheckFilesOnStartup() const
{
    return( value( "FilmsList/CheckFilesOnStartup", true ).toBool() );
}

int AlexandraSettings::GetColumnViewedWidth() const
{
    int defaultValue = 20;
    return( value( "FilmsList/ColumnViewedW", defaultValue ).toInt() );
}

int AlexandraSettings::GetColumnFavouriteWidth() const
{
    int defaultValue = 20;
    return( value( "FilmsList/ColumnFavouriteW", defaultValue ).toInt() );
}

int AlexandraSettings::GetColumnTitleWidth() const
{
    int defaultValue = 150;
    return( value( "FilmsList/ColumnTitleW", defaultValue ).toInt() );
}

int AlexandraSettings::GetColumnYearWidth() const
{
    int defaultValue = 50;
    return( value( "FilmsList/ColumnYearW", defaultValue ).toInt() );
}

int AlexandraSettings::GetColumnGenreWidth() const
{
    int defaultValue = 110;
    return( value( "FilmsList/ColumnGenreW", defaultValue ).toInt() );
}

int AlexandraSettings::GetColumnDirectorWidth() const
{
    int defaultValue = 110;
    return( value( "FilmsList/ColumnDirectorW", defaultValue ).toInt() );
}

QString AlexandraSettings::GetCurrentFilmTitle() const
{
    return( value( "FilmsList/CurrentFilm", "" ).toString() );
}

QString AlexandraSettings::GetPostersDirPath() const
{
    return( value( "FilmsList/PostersDir", "" ).toString() );
}

int AlexandraSettings::GetScalePosterToHeight() const
{
    return( value( "FilmsList/ScalePosters", 600 ).toInt() );
}

QRgb AlexandraSettings::GetUnavailableFileColor() const
{
    return( value( "FilmsList/UnavailableFileColor", qRgb( 0xff, 0xc0, 0xc0 ) ).toUInt() );
}

int AlexandraSettings::GetGridViewItemSize() const
{
    return( value( "GridView/ItemSize", 130 ).toInt() );
}

int AlexandraSettings::GetGridViewTextSize() const
{
    return( value( "GridView/TextSize", 12 ).toInt() );
}

int AlexandraSettings::GetGridViewTextLength() const
{
    return( value( "GridView/TextLength", 25 ).toInt() );
}

int AlexandraSettings::GetGridViewColumnCount() const
{
    return( value( "GridView/ColumnCount", 3 ).toInt() );
}

bool AlexandraSettings::GetGridViewShowTooltip() const
{
    return( value( "GridView/ShowTooltip", false ).toBool() );
}

QByteArray AlexandraSettings::GetMainWindowGeometry() const
{
    return( value( "MainWindow/Geometry" ).toByteArray() );
}

QByteArray AlexandraSettings::GetMainWindowState() const
{
    return( value( "MainWindow/State" ).toByteArray() );
}

QByteArray AlexandraSettings::GetMainWindowSplitterState() const
{
    return( value( "MainWindow/SplitterState" ).toByteArray() );
}

int AlexandraSettings::GetMainWindowToolbarStyle() const
{
    return( value( "MainWindow/ToolbarStyle", Qt::ToolButtonFollowStyle ).toInt() );
}

// Set //

void AlexandraSettings::SetDatabaseFilePath( const QString& s )
{
    setValue( "Application/DatabaseFile", s );
}

void AlexandraSettings::SetExternalPlayer( const QString& s )
{
    setValue( "Application/ExternalPlayer", s );
}

void AlexandraSettings::SetLastFilmPath( const QString& s )
{
    setValue( "Application/LastFilmPath", s );
}

void AlexandraSettings::SetLastPosterPath( const QString& s )
{
    setValue( "Application/LastPosterPath", s );
}

void AlexandraSettings::SetApplicationLocaleIndex( int n )
{
    setValue( "Application/Locale", n );
}

void AlexandraSettings::SetApplicationStyleName( const QString& s )
{
    setValue( "Application/Style", s );
}

void AlexandraSettings::SetFilmsViewMode( int n )
{
    setValue( "FilmsView/Mode", n );
}

void AlexandraSettings::SetCheckFilesOnStartup( bool b )
{
    setValue( "FilmsList/CheckFilesOnStartup", b );
}

void AlexandraSettings::SetColumnViewedWidth( int n )
{
    setValue( "FilmsList/ColumnViewedW", n );
}

void AlexandraSettings::SetColumnFavouriteWidth( int n )
{
    setValue( "FilmsList/ColumnFavouriteW", n );
}

void AlexandraSettings::SetColumnTitleWidth( int n )
{
    setValue( "FilmsList/ColumnTitleW", n );
}

void AlexandraSettings::SetColumnYearWidth( int n )
{
    setValue( "FilmsList/ColumnYearW", n );
}

void AlexandraSettings::SetColumnGenreWidth( int n )
{
    setValue( "FilmsList/ColumnGenreW", n );
}

void AlexandraSettings::SetColumnDirectorWidth( int n )
{
    setValue( "FilmsList/ColumnDirectorW", n );
}

void AlexandraSettings::SetCurrentFilmTitle( const QString& s )
{
    setValue( "FilmsList/CurrentFilm", s );
}

void AlexandraSettings::SetPostersDirPath( const QString& s )
{
    setValue( "FilmsList/PostersDir", s );
}

void AlexandraSettings::SetScalePostersToHeight( int n )
{
    setValue( "FilmsList/ScalePosters", n );
}

void AlexandraSettings::SetUnavailableFileColor( QRgb c )
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

void AlexandraSettings::SetMainWindowSplitterState( const QByteArray& a )
{
    setValue( "MainWindow/SplitterState", a );
}

void AlexandraSettings::SetMainWindowToolbarStyle( int n )
{
    setValue( "MainWindow/ToolbarStyle", n );
}
