/*************************************************************************************************
 *                                                                                                *
 *  file: alexandrasettings.cpp                                                                   *
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

#include "alexandrasettings.h"
#include "filmslist/filmitem.h"
#include "tools/debug.h"
#include "version.h"

#include <QApplication>
#include <QFileInfo>
#include <QFont>
#include <QDir>


AlexandraSettings* AlexandraSettings::instance = nullptr;


void AlexandraSettings::Initialize( const QString& configFile )
{
    instance = new AlexandraSettings( configFile );
}


AlexandraSettings* AlexandraSettings::GetInstance()
{
    return( instance );
}


  /* Get */

QString AlexandraSettings::GetDatabaseFilePath() const
{
    QString databasePathDefault( QFileInfo( this->fileName() ).absolutePath() + "/database.adat" );

    // In portable version of the program the database file and the posters directory
    // located in the same directory as the configuration file

    #ifdef PORTABLE_VERSION
        return( databasePathDefault );
    #else
        return( value( "Application/DatabaseFile", databasePathDefault ).toString() );
    #endif
}


QString AlexandraSettings::GetExternalPlayer() const
{
    QString externalPlayerDefault;

    #ifdef Q_OS_LINUX
        externalPlayerDefault = "xdg-open";
    #elif defined(Q_OS_WIN32)
        externalPlayerDefault = "C:\\Program Files\\Windows Media Player\\wmplayer.exe";
    #endif

    return( value( "Application/ExternalPlayer", externalPlayerDefault ).toString() );
}


QString AlexandraSettings::GetLastFilmPath() const
{
    #ifdef PORTABLE_VERSION
        QString defaultValue = "..";
    #else
        QString defaultValue = QDir::homePath();
    #endif

    return( value( "Application/LastFilmPath", defaultValue ).toString() );
}


QString AlexandraSettings::GetLastPosterPath() const
{
    return( value( "Application/LastPosterPath", QDir::homePath() ).toString() );
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


QByteArray AlexandraSettings::GetListViewHeaderState() const
{
    return( value( "ListView/HeaderState" ).toByteArray() );
}


QString AlexandraSettings::GetCurrentFilmTitle() const
{
    return( value( "FilmsList/CurrentFilm", "" ).toString() );
}


QString AlexandraSettings::GetPostersDirPath() const
{
    QString postersDirDefault = QFileInfo( this->fileName() ).absolutePath() + "/posters";

    // Comment in AlexandraSettings::GetDatabaseFilePath() function

    #ifdef PORTABLE_VERSION
        return( postersDirDefault );
    #else
        return( value( "FilmsList/PostersDir", postersDirDefault ).toString() );
    #endif
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
    unsigned int defaulValue = qRgb( 0xff, 0xc0, 0xc0 ); // Light red
    return( value( "FilmsList/UnavailableFileColor", defaulValue ).toUInt() );
}


int AlexandraSettings::GetListFontSize() const
{
    int defaultValue = qApp->font().pointSize();
    return( value( "ListView/FontSize", defaultValue ).toInt() );
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
    int defaultValue = qApp->font().pointSize();
    return( value( "GridView/FontSize", defaultValue ).toInt() );
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
    #ifdef Q_OS_LINUX
        int defaultValue = Qt::ToolButtonFollowStyle;
    #else
        int defaultValue = Qt::ToolButtonTextBesideIcon;
    #endif

    return( value( "MainWindow/ToolbarStyle", defaultValue ).toInt() );
}


bool AlexandraSettings::GetMainWindowShowRightPanel() const
{
    return( value( "MainWindow/ShowRightPanel", true ).toBool() );
}


int AlexandraSettings::GetMainWindowRightPanelWidth() const
{
    return( value( "MainWindow/RightPanelWidth", 200 ).toInt() );
}


bool AlexandraSettings::GetMainWindowStatusbarIsVisible() const
{
    return( value( "MainWindow/StatusbarIsVisible", true ).toBool() );
}


QList<int> AlexandraSettings::GetQuickSearchFilter() const
{
    QVariantList defaultValue = { FilmItem::TitleColumn, FilmItem::OriginalTitleColumn, FilmItem::TagsColumn };

    QVariantList list = value( "MainWindow/QuickSearchFilter", defaultValue ).toList();
    QList<int> result;

    for( const QVariant& i : list )
    {
        result.append( i.toInt() );
    }

    return( result );
}


bool AlexandraSettings::GetShowTechInfo() const
{
    return( value( "MainWindow/ShowTechInfo", false ).toBool() );
}


QString AlexandraSettings::GetFilmsScannerLastDir() const
{
    #ifdef PORTABLE_VERSION
        QString defaultValue = "..";
    #else
        QString defaultValue = QDir::homePath();
    #endif

    return( value( "FilmsScanner/LastDir", defaultValue ).toString() );
}


int AlexandraSettings::GetDefaultParserIndex() const
{
    return( value( "FilmsScanner/DefaultParserIndex", 0 ).toInt() ); // Parser::Auto
}


bool AlexandraSettings::GetParsersLoadAdvancedInfo() const
{
    return( value( "Parsers/LoadAdvancedInfo", true ).toBool() );
}


bool AlexandraSettings::GetParsersLoadBigPoster() const
{
    return( value( "Parsers/LoadBigPoster", true ).toBool() );
}


bool AlexandraSettings::GetParsersProxyEnabled() const
{
    return( value( "Parsers/ProxyEnabled", false ).toBool() );
}


QString AlexandraSettings::GetParsersProxyHostname() const
{
    return( value( "Parsers/ProxyHostname", "" ).toString() );
}


int AlexandraSettings::GetParsersProxyPort() const
{
    return( value( "Parsers/ProxyPort", "" ).toInt() );
}


QString AlexandraSettings::GetParsersProxyUsername() const
{
    return( value( "Parsers/ProxyUsername", "" ).toString() );
}


QString AlexandraSettings::GetParsersProxyPassword() const
{
    return( value( "Parsers/ProxyPassword", "" ).toString() );
}


QString AlexandraSettings::GetParsersOmdbApiKey() const
{
    return( value( "Parsers/OmdbApiKey", "" ).toString() );
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
    return( value( "Player/DoubleClickBehavior", QString() ).toString() );
}


QByteArray AlexandraSettings::GetPlaylistData() const
{
    return( value( "MainWindow/PlaylistData" ).toByteArray() );
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


void AlexandraSettings::SetListViewHeaderState( const QByteArray& a )
{
    setValue( "ListView/HeaderState", a );
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


void AlexandraSettings::SetMainWindowStatusbarIsVisible( bool b )
{
    setValue( "MainWindow/StatusbarIsVisible", b );
}


void AlexandraSettings::SetQuickSearchFilter( QList<int> list )
{
    QVariantList data;

    for( int i : list )
    {
        data.append( i );
    }

    setValue( "MainWindow/QuickSearchFilter", data );
}


void AlexandraSettings::SetShowTechInfo( bool b )
{
    setValue( "MainWindow/ShowTechInfo", b );
}


void AlexandraSettings::SetFilmsScannerLastDir( const QString& s )
{
    setValue( "FilmsScanner/LastDir", s );
}


void AlexandraSettings::SetDefaultParserIndex( int n )
{
    setValue( "FilmsScanner/DefaultParserIndex", n );
}


void AlexandraSettings::SetParsersLoadAdvancedInfo( bool b )
{
    setValue( "Parsers/LoadAdvancedInfo", b );
}


void AlexandraSettings::SetParsersLoadBigPoster( bool b )
{
    this->setValue( "Parsers/LoadBigPoster", b );
}


void AlexandraSettings::SetParsersProxyEnabled( bool b )
{
    this->setValue( "Parsers/ProxyEnabled", b );
}


void AlexandraSettings::SetParsersProxyHostname( const QString& s )
{
    this->setValue( "Parsers/ProxyHostname", s );
}


void AlexandraSettings::SetParsersProxyPort( int n )
{
    this->setValue( "Parsers/ProxyPort", n );
}


void AlexandraSettings::SetParsersProxyUsername( const QString& s )
{
    this->setValue( "Parsers/ProxyUsername", s );
}


void AlexandraSettings::SetParsersProxyPassword( const QString& s )
{
    this->setValue( "Parsers/ProxyPassword", s );
}


void AlexandraSettings::SetParsersOmdbApiKey( const QString& s)
{
    this->setValue( "Parsers/OmdbApiKey", s );
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


void AlexandraSettings::SetPlaylistData( const QByteArray& a )
{
    setValue( "MainWindow/PlaylistData", a );
}


AlexandraSettings::AlexandraSettings( const QString& configFile )
    : QSettings( configFile, QSettings::IniFormat )
{
    DebugPrint( "Config filename: " + this->fileName() );

    quint8 version = value( "Application/SettingsVersion" ).toUInt();

    if( version != Alexandra::settingsVersion )
    {
          // Remove old keys
        QMap<QString,QStringList> oldKeys =
        {
            { "FilmsList", QStringList {
                  "ColumnDirectorW",
                  "ColumnFavouriteW",
                  "ColumnGenreW",
                  "ColumnTitleW",
                  "ColumnViewedW",
                  "ColumnYearW"
              }
            },
            { "ListView", QStringList {
                  "ColumnDirectorW",
                  "ColumnFavouriteW",
                  "ColumnGenreW",
                  "ColumnTitleW",
                  "ColumnViewedW",
                  "ColumnYearW"
              }
            }
        };

        for( const QString& key : oldKeys.keys() )
        {
            for( const QString& value : oldKeys.value(key) )
            {
                remove( key + "/" + value );
            }
        }

          //Save
        setValue( "Application/SettingsVersion", Alexandra::settingsVersion );
        this->sync();
    }
}

