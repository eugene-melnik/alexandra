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
#include "tools/debug.h"
#include "version.h"

#include <QApplication>
#include <QFileInfo>
#include <QFont>

AlexandraSettings::AlexandraSettings( const QString& configFile ) : QSettings( configFile, QSettings::IniFormat )
{
    DebugPrintFuncA( "AlexandraSettings::AlexandraSettings", configFile );

    // In portable version of the program the database file and the posters directory
    // located in the same directory as the configuration file
#ifdef PORTABLE_VERSION
    SetDatabaseFilePath( QFileInfo( this->fileName() ).absolutePath() + "/database.adat" );
    SetPostersDirPath( QFileInfo( this->fileName() ).absolutePath() + "/posters" );
#else

    // Set database filename
    QString databaseFileName = GetDatabaseFilePath();

    if( databaseFileName.isEmpty() )
    {
        databaseFileName = QFileInfo( this->fileName() ).absolutePath() + "/database.adat";
        SetDatabaseFilePath( databaseFileName );
    }

    // Set directory of the posters
    QString postersFolderName = GetPostersDirPath();

    if( postersFolderName.isEmpty() )
    {
        postersFolderName = QFileInfo( databaseFileName ).absolutePath() + "/posters";
        SetPostersDirPath( postersFolderName );
    }
#endif

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

    DebugPrint( "Database: " + databaseFileName );
    DebugPrint( "Posters dir: " + postersFolderName );
    DebugPrint( "External player: " + externalPlayerName );

    sync();
}

/* Get */

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
    return( value( "Application/LastFilmPath", "" ).toString() );
}

QString AlexandraSettings::GetLastPosterPath() const
{
    return( value( "Application/LastPosterPath", "" ).toString() );
}

int AlexandraSettings::GetApplicationLocaleIndex() const
{
    return( value( "Application/Locale", -1 ).toInt() );
}

QString AlexandraSettings::GetApplicationFont() const
{
    return( value( "Application/Font", qApp->font().toString() ).toString() );
}

QString AlexandraSettings::GetApplicationStyleName() const
{
    return( value( "Application/Style" ).toString() );
}

int AlexandraSettings::GetApplicationThemeIndex() const
{
    return( value( "Application/ThemeIndex" ).toInt() );
}

bool AlexandraSettings::GetApplicationShowSplashScreen() const
{
    return( value( "Application/ShowSplashScreen", true ).toBool() );
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
    // Default value here and some values below
    // are based on scientific calculations xD
    int defaultValue = 20;
    return( value( "ListView/ColumnViewedW", defaultValue ).toInt() );
}

int AlexandraSettings::GetColumnFavouriteWidth() const
{
    int defaultValue = 20;
    return( value( "ListView/ColumnFavouriteW", defaultValue ).toInt() );
}

int AlexandraSettings::GetColumnTitleWidth() const
{
    int defaultValue = 150;
    return( value( "ListView/ColumnTitleW", defaultValue ).toInt() );
}

int AlexandraSettings::GetColumnYearWidth() const
{
    int defaultValue = 50;
    return( value( "ListView/ColumnYearW", defaultValue ).toInt() );
}

int AlexandraSettings::GetColumnGenreWidth() const
{
    int defaultValue = 110;
    return( value( "ListView/ColumnGenreW", defaultValue ).toInt() );
}

int AlexandraSettings::GetColumnDirectorWidth() const
{
    int defaultValue = 110;
    return( value( "ListView/ColumnDirectorW", defaultValue ).toInt() );
}

QString AlexandraSettings::GetCurrentFilmTitle() const
{
    return( value( "FilmsList/CurrentFilm", "" ).toString() );
}

QString AlexandraSettings::GetPostersDirPath() const
{
    return( value( "FilmsList/PostersDir", "" ).toString() );
}

QString AlexandraSettings::GetPosterSavingFormat() const
{
    return( value( "FilmsList/PosterSavingFormat", "jpg" ).toString() );
}

int AlexandraSettings::GetPosterSavingQuality() const
{
    return( value( "FilmsList/PosterSavingQuality", 95 ).toInt() );
}

int AlexandraSettings::GetScalePosterToHeight() const
{
    return( value( "FilmsList/ScalePosters", 800 ).toInt() );
}

QRgb AlexandraSettings::GetUnavailableFileColor() const
{
    unsigned int defaulValue = qRgb( 0xff, 0xc0, 0xc0 ); // Light red color
    return( value( "FilmsList/UnavailableFileColor", defaulValue ).toUInt() );
}

int AlexandraSettings::GetListFontSize() const
{
    return( value( "ListView/FontSize", 10 ).toInt() ); // In Windows by default font size if '8'...
                                                        // Need to do something
}

int AlexandraSettings::GetListRowHeight() const
{
    return( value( "ListView/RowHeight", 20 ).toInt() );
}

int AlexandraSettings::GetGridItemSize() const
{
    return( value( "GridView/ItemSize", 130 ).toInt() ); // 130 is the best value :D
}

int AlexandraSettings::GetGridFontSize() const
{
    return( value( "GridView/FontSize", 10 ).toInt() ); // Font size. Again ^
}

bool AlexandraSettings::GetGridShowTooltip() const
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

bool AlexandraSettings::GetMainWindowShowRightPanel() const
{
    return( value( "MainWindow/ShowRightPanel", true ).toBool() );
}

int AlexandraSettings::GetMainWindowRightPanelWidth() const
{
    return( value( "MainWindow/RightPanelWidth", 200 ).toInt() );
}

int AlexandraSettings::GetQuickSearchFilter() const
{
    return( value( "MainWindow/QuickSearchFilter", 1 ).toInt() ); // FilterBy::Title by default
}

bool AlexandraSettings::GetAutoLoadTechInfo() const
{
    return( value( "MainWindow/AutoLoadTechInfo", true ).toBool() );
}

QString AlexandraSettings::GetFilmsScannerLastDir() const
{
    return( value( "FilmsScanner/LastDir", "" ).toString() );
}

int AlexandraSettings::GetDefaultParserIndex() const
{
    return( value( "FilmsScanner/DefaultParserIndex", 0 ).toInt() ); // Parser::Auto
}

QString AlexandraSettings::GetShortcutAddFilm() const
{
    return( value( "Shortcuts/AddFilm", "Ctrl+A" ).toString() );
}

QString AlexandraSettings::GetShortcutEditFilm() const
{
    return( value( "Shortcuts/EditFilm", "Ctrl+E" ).toString() );
}

QString AlexandraSettings::GetShortcutRemoveFilm() const
{
    return( value( "Shortcuts/RemoveFilm", "Ctrl+Del" ).toString() );
}

QString AlexandraSettings::GetShortcutSelectRandomFilm() const
{
    return( value( "Shortcuts/SelectRandomFilm", "Ctrl+R" ).toString() );
}

QString AlexandraSettings::GetShortcutActivateQuickSearch() const
{
    return( value( "Shortcuts/ActivateQuickSearch", "Backspace" ).toString() );
}

QString AlexandraSettings::GetShortcutAdvancedSearch() const
{
    return( value( "Shortcuts/AdvancedSearch", "Ctrl+F" ).toString() );
}

QString AlexandraSettings::GetShortcutPlay() const
{
    return( value( "Shortcuts/Play", "Alt+Return" ).toString() );
}

QString AlexandraSettings::GetShortcutSettings() const
{
    return( value( "Shortcuts/Settings", "Ctrl+P" ).toString() );
}

QString AlexandraSettings::GetShortcutShowToolbar() const
{
    return( value( "Shortcuts/ShowToolbar", "Ctrl+T" ).toString() );
}

QString AlexandraSettings::GetShortcutShowFullscreen() const
{
    return( value( "Shortcuts/ShowFullscreen", "F11" ).toString() );
}

QString AlexandraSettings::GetShortcutExit() const
{
    return( value( "Shortcuts/Exit", "Ctrl+Q" ).toString() );
}

QString AlexandraSettings::GetPlayerDoubleClickBehavior() const
{
    return( value( "Player/DoubleClickBehavior", "auto" ).toString() );
}

/* Set */

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

void AlexandraSettings::SetApplicationFont( const QString& s )
{
    setValue( "Application/Font", s );
}

void AlexandraSettings::SetApplicationStyleName( const QString& s )
{
    setValue( "Application/Style", s );
}

void AlexandraSettings::SetApplicationThemeIndex( int n )
{
    setValue( "Application/ThemeIndex", n );
}

void AlexandraSettings::SetApplicationShowSplashScreen( bool b )
{
    setValue( "Application/ShowSplashScreen", b );
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
    setValue( "ListView/ColumnViewedW", n );
}

void AlexandraSettings::SetColumnFavouriteWidth( int n )
{
    setValue( "ListView/ColumnFavouriteW", n );
}

void AlexandraSettings::SetColumnTitleWidth( int n )
{
    setValue( "ListView/ColumnTitleW", n );
}

void AlexandraSettings::SetColumnYearWidth( int n )
{
    setValue( "ListView/ColumnYearW", n );
}

void AlexandraSettings::SetColumnGenreWidth( int n )
{
    setValue( "ListView/ColumnGenreW", n );
}

void AlexandraSettings::SetColumnDirectorWidth( int n )
{
    setValue( "ListView/ColumnDirectorW", n );
}

void AlexandraSettings::SetCurrentFilmTitle( const QString& s )
{
    setValue( "FilmsList/CurrentFilm", s );
}

void AlexandraSettings::SetPostersDirPath( const QString& s )
{
    setValue( "FilmsList/PostersDir", s );
}

void AlexandraSettings::SetPosterSavingFormat( const QString& s )
{
    setValue( "FilmsList/PosterSavingFormat", s );
}

void AlexandraSettings::SetPosterSavingQuality( int n )
{
    setValue( "FilmsList/PosterSavingQuality", n );
}

void AlexandraSettings::SetScalePostersToHeight( int n )
{
    setValue( "FilmsList/ScalePosters", n );
}

void AlexandraSettings::SetUnavailableFileColor( QRgb c )
{
    setValue( "FilmsList/UnavailableFileColor", c );
}

void AlexandraSettings::SetListFontSize( int n )
{
    setValue( "ListView/FontSize", n );
}

void AlexandraSettings::SetListRowHeight( int n )
{
    setValue( "ListView/RowHeight", n );
}

void AlexandraSettings::SetGridItemSize( int n )
{
    setValue( "GridView/ItemSize", n );
}

void AlexandraSettings::SetGridFontSize( int n )
{
    setValue( "GridView/FontSize", n );
}

void AlexandraSettings::SetGridShowTooltip( bool b )
{
    setValue( "GridView/ShowTooltip", b );
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

void AlexandraSettings::SetMainWindowShowRightPanel( bool b )
{
    setValue( "MainWindow/ShowRightPanel", b );
}

void AlexandraSettings::SetMainWindowRightPanelWidth( int n )
{
    setValue( "MainWindow/RightPanelWidth", n );
}

void AlexandraSettings::SetQuickSearchFilter( int n )
{
    setValue( "MainWindow/QuickSearchFilter", n );
}

void AlexandraSettings::SetAutoLoadTechInfo( bool b )
{
    setValue( "MainWindow/AutoLoadTechInfo", b );
}

void AlexandraSettings::SetFilmsScannerLastDir( const QString& s )
{
    setValue( "FilmsScanner/LastDir", s );
}

void AlexandraSettings::SetDefaultParserIndex( int n )
{
    setValue( "FilmsScanner/DefaultParserIndex", n );
}

void AlexandraSettings::SetShortcutAddFilm( const QString& s )
{
    setValue( "Shortcuts/AddFilm", s );
}

void AlexandraSettings::SetShortcutEditFilm( const QString& s )
{
    setValue( "Shortcuts/EditFilm", s );
}

void AlexandraSettings::SetShortcutRemoveFilm( const QString& s )
{
    setValue( "Shortcuts/RemoveFilm", s );
}

void AlexandraSettings::SetShortcutSelectRandomFilm( const QString& s )
{
    setValue( "Shortcuts/SelectRandomFilm", s );
}

void AlexandraSettings::SetShortcutActivateQuickSearch( const QString& s )
{
    setValue( "Shortcuts/ActivateQuickSearch", s );
}

void AlexandraSettings::SetShortcutAdvancedSearch( const QString& s )
{
    setValue( "Shortcuts/AdvancedSearch", s );
}

void AlexandraSettings::SetShortcutPlay( const QString& s )
{
    setValue( "Shortcuts/Play", s );
}

void AlexandraSettings::SetShortcutSettings( const QString& s )
{
    setValue( "Shortcuts/Settings", s );
}

void AlexandraSettings::SetShortcutShowToolbar( const QString& s )
{
    setValue( "Shortcuts/ShowToolbar", s );
}

void AlexandraSettings::SetShortcutShowFullscreen( const QString& s )
{
    setValue( "Shortcuts/ShowFullscreen", s );
}

void AlexandraSettings::SetShortcutExit( const QString& s )
{
    setValue( "Shortcuts/Exit", s );
}

void AlexandraSettings::SetPlayerDoubleClickBehavior( const QString& s )
{
    setValue( "Player/DoubleClickBehavior", s );
}
