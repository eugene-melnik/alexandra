/*************************************************************************************************
 *                                                                                                *
 *  file: kinoteatrparser.cpp                                                                     *
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
#include "kinoteatrparser.h"
#include "tools/regexptools.h"
#include "tools/debug.h"


KinoteatrParser::KinoteatrParser() : AbstractParser()
{
    DebugPrintFunc( "KinoteatrParser::KinoteatrParser" );
    searchUrlWithYear = QString( "http://kino-teatr.ua/uk/main/films.phtml?title=%1&year=%2" );
    searchUrl = QString( "http://kino-teatr.ua/uk/main/films.phtml?title=%1" );
}


QUrl KinoteatrParser::Parse( const QByteArray& data )
{
    DebugPrintFunc( "KinoteatrParser::Parse", data.size() );

    QString foundedPage( data );
    RegExpTools::SimplifyText( foundedPage );

    QRegExp reRedirect( "id='searchItemFilms'><a href='(.*)'");
    QString redirectUrl( RegExpTools::ParseItem( foundedPage, reRedirect ) );

    QUrl posterUrl;

    if( !redirectUrl.isEmpty() )
    {
        QString str( request.runSync( QUrl( redirectUrl ) ) );
        RegExpTools::SimplifyText( str );
        DebugPrint( QString( "Simpified to: %1 bytes" ).arg( str.size() ) );

          // Title
        QRegExp reTitle( "class='red' itemprop=\"name\">(.*)</strong>" );
        film.SetColumnData( FilmItem::TitleColumn, RegExpTools::ParseItem( str, reTitle ) );

          // Original title
        QRegExp reOriginalTitle( "class='grey' itemprop=\"alternativeHeadline\">(.*)</span>" );
        film.SetColumnData( FilmItem::OriginalTitleColumn, RegExpTools::ParseItem( str, reOriginalTitle ) );

          // Year
        QRegExp reYear( "Рік: </strong><.*>(.*)</a>" );
        film.SetColumnData( FilmItem::YearColumn, RegExpTools::ParseItem( str, reYear ).toInt() );

          // Rating
        QRegExp reRating( "id=\"rating\">(.*) \\/" );
        film.SetColumnData( FilmItem::RatingColumn, RegExpTools::ParseItem( str, reRating ).replace( ",", "." ).toDouble() );

          // Country
        QRegExp reCountryList( "Країна:(.*)<br>" );
        QRegExp reName( "<a.*>(.*)</a>" );
        film.SetColumnData( FilmItem::CountryColumn, RegExpTools::ParseList( str, reCountryList, reName ) );

          // Director
        QRegExp reDirectorList( "Режисери:(.*)</span>" );
        film.SetColumnData( FilmItem::DirectorColumn, RegExpTools::ParseList( str, reDirectorList, reName ) );

          // Genre
        QRegExp reGenreList( "Жанр:(.*)</span>" );
        film.SetColumnData( FilmItem::GenreColumn, RegExpTools::ParseList( str, reGenreList, reName ) );

          // Starring
        QRegExp reStarringList( "В ролях:(.*)</ul>" );
        reName = QRegExp( "<a.*>(.*)</a></span>" );
        film.SetColumnData( FilmItem::StarringColumn, RegExpTools::ParseList( str, reStarringList, reName ) );

          // Description
        QRegExp reDescription( "id='filmText' itemprop=\"description\"><.*>(.*)</p>" );
        film.SetColumnData( FilmItem::DescriptionColumn, RegExpTools::ParseItem( str, reDescription ) );

          // Advanced information

        if( AlexandraSettings::GetInstance()->GetParsersLoadAdvancedInfo() )
        {
            QString newRedirectUrl = redirectUrl;
            QString str( request.runSync( QUrl( newRedirectUrl.replace( "film", "film-persons" ) ) ) );
            RegExpTools::SimplifyText( str );
            DebugPrint( QString( "Simpified to: %1 bytes" ).arg( str.size() ) );

              // Screenwriter
            QRegExp reScreenwriterList( "сценаристи(.*)</ul></div>" );
            reName = QRegExp( "<div class='liderName'><a.*>(.*)</a>" );
            film.SetColumnData( FilmItem::ScreenwriterColumn, RegExpTools::ParseList( str, reScreenwriterList, reName ) );

              // Producer
            QRegExp reProducerList( "продюсери(.*)</ul></div>" );
            film.SetColumnData( FilmItem::ProducerColumn, RegExpTools::ParseList( str, reProducerList, reName ) );

              // Composer
            QRegExp reComposerList( "композитори(.*)</ul></div>" );
            film.SetColumnData( FilmItem::ComposerColumn, RegExpTools::ParseList( str, reComposerList, reName ) );
        }

          // Poster

        if( AlexandraSettings::GetInstance()->GetParsersLoadBigPoster() )
        {
              // Lot of redirectes... :(

            QString newRedirectUrl = redirectUrl;
            str = QString( request.runSync( QUrl( newRedirectUrl.replace( "film", "film-poster" ) ) ) );
            RegExpTools::SimplifyText( str );
            DebugPrint( QString( "Simpified to: %1 bytes" ).arg( str.size() ) );

            QRegExp rePoster( "class=\"poster_preview\" ><a.*href=\"(.*)\"" );
            str = QString( request.runSync( QUrl( RegExpTools::ParseItem( str, rePoster ) ) ) );
            RegExpTools::SimplifyText( str );
            DebugPrint( QString( "Simpified to: %1 bytes" ).arg( str.size() ) );

            rePoster = QRegExp( "class=\"posterImg\".*><img src=\"(.*)\"" );
            posterUrl = RegExpTools::ParseItem( str, rePoster );
        }
        else // Small poster
        {
            QRegExp rePoster( "id='filmPoster'.*><img src='(.*)'" );
            posterUrl = RegExpTools::ParseItem( str, rePoster );
        }

        DebugPrint( "Text parsed!" );
        emit Loaded( film, posterUrl );
    }
    else
    {
        emit Error( "Movie not found!" );
    }

    return( posterUrl );
}

