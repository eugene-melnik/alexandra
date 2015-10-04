/*************************************************************************************************
 *                                                                                                *
 *  file: kinopoiskparser.cpp                                                                     *
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

#include "kinopoiskparser.h"
#include "debug.h"

#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QTextCodec>

KinopoiskParser::KinopoiskParser() : AbstractParser()
{
    DebugPrintFunc( "KinopoiskParser::KinopoiskParser" );
}

void KinopoiskParser::SearchFor( const QString& title, const QString& year )
{
    searchUrlWithYear = QString( "http://www.kinopoisk.ru/index.php?level=7&m_act[what]=content&first=yes&m_act[find]=%1&m_act[year]=%2" );
    searchUrl = QString( "http://www.kinopoisk.ru/index.php?first=yes&what=&kp_query=%1" );
    AbstractParser::SearchFor( title, year );
}

void KinopoiskParser::SyncSearchFor( Film * filmSaveTo, QUrl* posterUrlSaveTo,
                                     const QString& title, const QString& year )
{
    searchUrlWithYear = QString( "http://www.kinopoisk.ru/index.php?level=7&m_act[what]=content&first=yes&m_act[find]=%1&m_act[year]=%2" );
    searchUrl = QString( "http://www.kinopoisk.ru/index.php?first=yes&what=&kp_query=%1" );
    AbstractParser::SyncSearchFor( filmSaveTo, posterUrlSaveTo, title, year );
}

QUrl KinopoiskParser::Parse( const QByteArray& data )
{
    DebugPrintFuncA( "KinopoiskParser::Parse", data.size() );

    QTextCodec* codec = QTextCodec::codecForName( "Windows-1251" );
    QString str = codec->toUnicode( data );

    str = str.simplified();
    str.replace( "&nbsp;",  " " );
    str.replace( "&raquo;", "»" );
    str.replace( "&laquo;", "«" );
    str.replace( "&lt;",    "<" );
    str.replace( "&gt;",    ">" );
    str.replace( "&quot;",  "\"" );
    str.replace( "&#x27;",  "'" );
    str.replace( "%5B",     "[" );
    str.replace( "%5D",     "]" );
    str.replace( "%20",     " " );
    str.replace( "&#133;",  "…" );
    str.replace( 0x0A,      "" );
    str.replace( "> <",     "><" );

    DebugPrint( "Simpified to: " + QString::number( str.size() ) );

// TODO: case if not founded

    // Title
    QRegExp reTitle( "class=\"moviename-big\".*>(.*)</h1>" );
    film.SetTitle( ParseItem( str, reTitle ) );

    // Original title
    QRegExp reOriginalTitle( "itemprop=\"alternativeHeadline\">(.*)</span>" );
    film.SetOriginalTitle( ParseItem( str, reOriginalTitle ) );

    // Tagline
    QRegExp reTagline( "слоган</td><.*>(.*)</td>" );
    film.SetTagline( ParseItem( str, reTagline ) );

    // Year
    QRegExp reYear( "href=\"/lists/m_act\\[year\\]/.*>(.*)</a>" );
    film.SetYearFromStr( ParseItem( str, reYear ) );

    // Budget
    QRegExp reBudget( "бюджет</td><.*><.*><.*>\\$(.*)</a>" );
    film.SetBudgetFromStr( ParseItem( str, reBudget ).replace( " ", "" ) );

    // Rating
    QRegExp reRating( "class=\"rating_ball\">(.*)</span>" );
    film.SetRatingFromStr( ParseItem( str, reRating ) );

    // Country
    QRegExp reCountryList( "страна</td><td><.*>(.*)</td>" );
    QRegExp reCountry( "href=\"/lists/m_act\\[country\\]/.*>(.*)</a>" );
    film.SetCountry( ParseList( str, reCountryList, reCountry ) );

    // Director
    QRegExp reDirectorList( "itemprop=\"director\">(.*)</td>" );
    QRegExp reName( "href=\"/name/.*>(.*)</a>" );
    film.SetDirector( ParseList( str, reDirectorList, reName ) );

    // Screenwriter
    QRegExp reScreenwriterList( "сценарий</td><.*>(.*)</td>" );
    film.SetScreenwriter( ParseList( str, reScreenwriterList, reName ) );

    // Genre
    QRegExp reGenreList( "itemprop=\"genre\">(.*)</span>" );
    QRegExp reGenre( "href=\"/lists/.*>(.*)</a>" );
    film.SetGenre( ParseList( str, reGenreList, reGenre ) );

    // Producer
    QRegExp reProducerList( "itemprop=\"producer\">(.*)</td>" );
    film.SetProducer( ParseList( str, reProducerList, reName ) );

    // Composer
    QRegExp reComposerList( "itemprop=\"musicBy\">(.*)</td>" );
    film.SetComposer( ParseList( str, reComposerList, reName ) );

    // Starring
    QRegExp reStarringList( "В главных ролях:</h4>(.*)</ul>" );
    film.SetStarring( ParseList( str, reStarringList, reName ) );

    // Description
    QRegExp reDescription( "itemprop=\"description\">(.*)</div>" );
    film.SetDescription( ParseItem( str, reDescription ).replace( "<br><br>", "<br>\n" ) );

    DebugPrint( "Text parsed!" );

    // Poster
    QRegExp rePosterUrl( "openImgPopup\\('(.*)'\\)" );
    QString posterSubUrl = ParseItem( str, rePosterUrl );

    if( !posterSubUrl.isEmpty() )
    {
        QUrl posterUrl( "http://www.kinopoisk.ru" + posterSubUrl );
        emit Loaded( film, posterUrl );
        return( posterUrl );
    }

    emit Loaded( film, QUrl() );
    return( QString() );
}

QString KinopoiskParser::ParseList( const QString& str, QRegExp& reList, QRegExp& reItem ) const
{
    reItem.setMinimal( true );
    reList.setMinimal( true );
    reList.indexIn( str );

    QStringList result;
    int nextPosition = 0;

    while( reItem.indexIn( reList.cap(1), nextPosition ) != -1 )
    {
        result.append( reItem.cap(1).trimmed() );
        nextPosition += reItem.matchedLength();
    }

    return( result.join( ", " ) );
}

QString KinopoiskParser::ParseItem( const QString& str, QRegExp& reItem ) const
{
    reItem.setMinimal( true );
    reItem.indexIn( str );
    return( reItem.cap(1).trimmed() );
}
