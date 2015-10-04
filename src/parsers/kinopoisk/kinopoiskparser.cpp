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
#include "tools/regexptools.h"
#include "debug.h"

#include <QByteArray>
#include <QTextCodec>

KinopoiskParser::KinopoiskParser() : AbstractParser()
{
    DebugPrintFunc( "KinopoiskParser::KinopoiskParser" );
    searchUrlWithYear = QString( "http://www.kinopoisk.ru/index.php?level=7&m_act[what]=content&first=yes&m_act[find]=%1&m_act[year]=%2" );
    searchUrl = QString( "http://www.kinopoisk.ru/index.php?first=yes&kp_query=%1" );
}

QUrl KinopoiskParser::Parse( const QByteArray& data )
{
    DebugPrintFuncA( "KinopoiskParser::Parse", data.size() );

    QTextCodec* codec = QTextCodec::codecForName( "Windows-1251" );
    QString str = codec->toUnicode( data );
    RegExpTools::SimplifyText( str );

    DebugPrint( QString( "Simpified to: %1 bytes" ).arg( str.size() ) );

    QUrl posterUrl;

    // Title
    QRegExp reTitle( "class=\"moviename-big\".*>(.*)</h1>" );
    film.SetTitle( RegExpTools::ParseItem( str, reTitle ) );

    if( !film.GetTitle().isEmpty() )
    {
        // Original title
        QRegExp reOriginalTitle( "itemprop=\"alternativeHeadline\">(.*)</span>" );
        film.SetOriginalTitle( RegExpTools::ParseItem( str, reOriginalTitle ) );

        // Tagline
        QRegExp reTagline( "слоган</td><.*>(.*)</td>" );
        film.SetTagline( RegExpTools::ParseItem( str, reTagline ) );

        // Year
        QRegExp reYear( "href=\"/lists/m_act\\[year\\]/.*>(.*)</a>" );
        film.SetYearFromStr( RegExpTools::ParseItem( str, reYear ) );

        // Budget
        QRegExp reBudget( "бюджет</td><.*><.*><.*>\\$(.*)</a>" );
        film.SetBudgetFromStr( RegExpTools::ParseItem( str, reBudget ).replace( " ", "" ) );

        // Rating
        QRegExp reRating( "class=\"rating_ball\">(.*)</span>" );
        film.SetRatingFromStr( RegExpTools::ParseItem( str, reRating ) );

        // Country
        QRegExp reCountryList( "страна</td><td><.*>(.*)</td>" );
        QRegExp reCountry( "href=\"/lists/m_act\\[country\\]/.*>(.*)</a>" );
        film.SetCountry( RegExpTools::ParseList( str, reCountryList, reCountry ) );

        // Director
        QRegExp reDirectorList( "itemprop=\"director\">(.*)</td>" );
        QRegExp reName( "href=\"/name/.*>(.*)</a>" );
        film.SetDirector( RegExpTools::ParseList( str, reDirectorList, reName ) );

        // Screenwriter
        QRegExp reScreenwriterList( "сценарий</td><.*>(.*)</td>" );
        film.SetScreenwriter( RegExpTools::ParseList( str, reScreenwriterList, reName ) );

        // Genre
        QRegExp reGenreList( "itemprop=\"genre\">(.*)</span>" );
        QRegExp reGenre( "href=\"/lists/.*>(.*)</a>" );
        film.SetGenre( RegExpTools::ParseList( str, reGenreList, reGenre ) );

        // Producer
        QRegExp reProducerList( "itemprop=\"producer\">(.*)</td>" );
        film.SetProducer( RegExpTools::ParseList( str, reProducerList, reName ) );

        // Composer
        QRegExp reComposerList( "itemprop=\"musicBy\">(.*)</td>" );
        film.SetComposer( RegExpTools::ParseList( str, reComposerList, reName ) );

        // Starring
        QRegExp reStarringList( "В главных ролях:</h4>(.*)</ul>" );
        film.SetStarring( RegExpTools::ParseList( str, reStarringList, reName ) );

        // Description
        QRegExp reDescription( "itemprop=\"description\">(.*)</div>" );
        film.SetDescription( RegExpTools::ParseItem( str, reDescription )
                             .replace( QRegExp( "<br /><br />|<br/><br/>|<br><br>" ), "<br/>\n" ) );

        DebugPrint( "Text parsed!" );

        // Poster
        QRegExp rePosterUrl( "openImgPopup\\('(.*)'\\)" );
        QString posterSubUrl = RegExpTools::ParseItem( str, rePosterUrl );

        if( !posterSubUrl.isEmpty() )
        {
            posterUrl = "http://www.kinopoisk.ru" + posterSubUrl;
        }

        emit Loaded( film, posterUrl );
    }
    else
    {
        emit Error( "Movie not found!" );
    }

    return( posterUrl );
}
