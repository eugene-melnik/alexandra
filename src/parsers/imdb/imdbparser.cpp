/*************************************************************************************************
 *                                                                                                *
 *  file: imdbparser.cpp                                                                          *
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

#include "imdbparser.h"
#include "tools/regexptools.h"
#include "debug.h"

#include <QByteArray>

#include<QFile>

ImdbParser::ImdbParser() : AbstractParser()
{
    DebugPrintFunc( "ImdbParser::ImdbParser" );
    searchUrlWithYear = QString( "http://www.imdb.com/search/title?title=%1&release_date=%2&view=simple" );
    searchUrl = QString( "http://www.imdb.com/search/title?title=%1&view=simple" );
}

QUrl ImdbParser::Parse( const QByteArray& data )
{
    DebugPrintFuncA( "ImdbParser::Parse", data.size() );

    QString foundedPage( data );
    RegExpTools::SimplifyText( foundedPage );

    QRegExp reRedirect( "class=\"number\">1.</td><td class=\"title\"><a href=\"(.*)\">");
    QString redirectUrl( RegExpTools::ParseItem( foundedPage, reRedirect ) );

    QUrl posterUrl;

    if( !redirectUrl.isEmpty() )
    {
        QString str( request.runSync( QUrl( "http://www.imdb.com" + redirectUrl ) ) );
        RegExpTools::SimplifyText( str );

        DebugPrint( QString( "Simpified to: %1 bytes" ).arg( str.size() ) );

        // Title
        QRegExp reTitle( "class=\"itemprop\".*>(.*)</span>" );
        film.SetTitle( RegExpTools::ParseItem( str, reTitle ) );

        // Original title
        QRegExp reOriginalTitle( "class=\"title-extra\".*> \"(.*)\" <i>" );
        film.SetOriginalTitle( RegExpTools::ParseItem( str, reOriginalTitle ) );

        // Tagline
        QRegExp reTagline( "Taglines:</h4>(.*)<span" );
        film.SetTagline( RegExpTools::ParseItem( str, reTagline ) );

        // Year
        QRegExp reYear( "Release Date:</h4>.*([0-9]{4})" );
        film.SetYearFromStr( RegExpTools::ParseItem( str, reYear ) );

        // Budget
        QRegExp reBudget( "Budget:</h4> \\$(.*)<span" );
        film.SetBudgetFromStr( RegExpTools::ParseItem( str, reBudget ).replace( " ", "" ) );

        // Rating
        QRegExp reRating( "itemprop=\"ratingValue\">(.*)</span>" );
        film.SetRatingFromStr( RegExpTools::ParseItem( str, reRating ).replace( ",", "." ) );

        // Country
        QRegExp reCountryList( "Country:</h4>(.*)</div>" );
        QRegExp reCountry( "href=\"/country/.*>(.*)</a>" );
        film.SetCountry( RegExpTools::ParseList( str, reCountryList, reCountry ) );

        // Director
        QRegExp reDirectorList( "Director:</h4>(.*)</div>" );
        QRegExp reName( "href=\"/name/.* itemprop='url'><.*>(.*)</span>" );
        film.SetDirector( RegExpTools::ParseList( str, reDirectorList, reName ) );

        // Screenwriter
        QRegExp reScreenwriterList( "Writers:</h4>(.*)</div>" );
        film.SetScreenwriter( RegExpTools::ParseList( str, reScreenwriterList, reName ) );

        // Genre
        QRegExp reGenreList( "Genres:</h4>(.*)</div>" );
        QRegExp reGenre( "href=\"/genre/.*>(.*)</a>" );
        film.SetGenre( RegExpTools::ParseList( str, reGenreList, reGenre ) );

        // Producer
        //QRegExp reProducerList( "" );
        //film.SetProducer( RegExpTools::ParseList( str, reProducerList, reName ) );

        // Composer
        //QRegExp reComposerList( "" );
        //film.SetComposer( RegExpTools::ParseList( str, reComposerList, reName ) );

        // Starring
        QRegExp reStarringList( "Cast</h2>(.*)</table>" );
        film.SetStarring( RegExpTools::ParseList( str, reStarringList, reName ) );

        // Description
        QRegExp reDescription( "Storyline</h2><.*><p>(.*)<em class=\"nobr\">" );
        film.SetDescription( RegExpTools::ParseItem( str, reDescription ).replace( "<br><br>", "<br>\n" ) );

        DebugPrint( "Text parsed!" );

        QRegExp rePoster( "<td.*id=\"img_primary\"><.*><a href=\"/media/.*><img.*src=\"(.*)\" itemprop=\"image\" />" );
        posterUrl = RegExpTools::ParseItem( str, rePoster );

        emit Loaded( film, posterUrl );
    }
    else
    {
        emit Error( "Movie not found!" );
    }

    return( QUrl() );
}
