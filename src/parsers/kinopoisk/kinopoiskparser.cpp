/*************************************************************************************************
 *                                                                                                *
 *  file: kinopoiskparser.cpp                                                                     *
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
#include "kinopoiskparser.h"
#include "tools/regexptools.h"
#include "tools/debug.h"

#include <QTextCodec>


KinopoiskParser::KinopoiskParser() : AbstractParser()
{
    DebugPrintFunc( "KinopoiskParser::KinopoiskParser" );
    searchUrlWithYear = QString( "http://www.kinopoisk.ru/index.php?level=7&m_act[what]=content&first=yes&m_act[find]=%1&m_act[year]=%2" );
    searchUrl = QString( "http://www.kinopoisk.ru/index.php?first=yes&kp_query=%1" );
}


QUrl KinopoiskParser::Parse( const QByteArray& data )
{
    DebugPrintFunc( "KinopoiskParser::Parse", data.size() );

    QTextCodec* codec = QTextCodec::codecForName( "Windows-1251" );
    QString str = codec->toUnicode( data );
    RegExpTools::SimplifyText( str );

    DebugPrint( QString( "Simpified to: %1 bytes" ).arg( str.size() ) );

    QUrl posterUrl;

      // Title
    QRegExp reTitle( "class=\"moviename-big\".*>(.*)</h1>" );
    film.SetColumnData( FilmItem::TitleColumn, RegExpTools::ParseItem( str, reTitle ) );

    if( !film.GetColumnData( FilmItem::TitleColumn ).toString().isEmpty() )
    {
          // Original title
        QRegExp reOriginalTitle( "itemprop=\"alternativeHeadline\">(.*)</span>" );
        film.SetColumnData( FilmItem::OriginalTitleColumn, RegExpTools::ParseItem( str, reOriginalTitle ) );

          // Tagline
        QRegExp reTagline( "слоган</td><.*>(.*)</td>" );
        QString tagline = RegExpTools::ParseItem( str, reTagline );

        if( tagline != "-" )
            film.SetColumnData( FilmItem::TaglineColumn, tagline );

          // Year
        QRegExp reYear( "href=\"/lists/m_act\\[year\\]/.*>(.*)</a>" );
        film.SetColumnData( FilmItem::YearColumn, RegExpTools::ParseItem( str, reYear ).toInt() );

          // Budget
        QRegExp reBudget( "бюджет</td><.*><.*><.*>\\$(.*)</a>" );
        film.SetColumnData( FilmItem::BudgetColumn, RegExpTools::ParseItem( str, reBudget ).replace( " ", "" ).toDouble() );

          // Rating
        QRegExp reRating( "class=\"rating_ball\">(.*)</span>" );
        film.SetColumnData( FilmItem::RatingColumn, RegExpTools::ParseItem( str, reRating ).toDouble() );

          // Country
        QRegExp reCountryList( "страна</td><td><.*>(.*)</td>" );
        QRegExp reCountry( "href=\"/lists/m_act\\[country\\]/.*>(.*)</a>" );
        film.SetColumnData( FilmItem::CountryColumn, RegExpTools::ParseList( str, reCountryList, reCountry ) );

          // Screenwriter
        QRegExp reScreenwriterList( "сценарий</td><.*>(.*)</td>" );
        QRegExp reName( "href=\"/name/.*>(.*)</a>" );
        film.SetColumnData( FilmItem::ScreenwriterColumn, RegExpTools::ParseList( str, reScreenwriterList, reName ) );

          // Genre
        QRegExp reGenreList( "itemprop=\"genre\">(.*)</span>" );
        QRegExp reGenre( "href=\"/lists/.*>(.*)</a>" );
        film.SetColumnData( FilmItem::GenreColumn, RegExpTools::ParseList( str, reGenreList, reGenre ) );

          // Description
        QRegExp reDescription( "itemprop=\"description\">(.*)</div>" );
        film.SetColumnData( FilmItem::DescriptionColumn, RegExpTools::ParseItem( str, reDescription )
                                                         .replace( QRegExp( "<br /><br />|<br/><br/>|<br><br>" ), "<br/>\n" ) );

          // Advanced information

        if( AlexandraSettings::GetInstance()->GetParsersLoadAdvancedInfo() )
        {
            QRegExp reMoreUrl( "<h4><a href=\"(.*)\">показать всех</a>" );
            QUrl moreUrl( "http://www.kinopoisk.ru" + RegExpTools::ParseItem( str, reMoreUrl ) );

            QString str = codec->toUnicode( request.runSync( moreUrl ) );
            RegExpTools::SimplifyText( str );
            DebugPrint( QString( "Simpified to: %1 bytes" ).arg( str.size() ) );

              // Director
            QRegExp reDirectorList( "Режиссер(.*)<a name" );
            reName = QRegExp( "class=\"name\"><a href=\"/name/.*>(.*)</a>" );
            film.SetColumnData( FilmItem::DirectorColumn, RegExpTools::ParseList( str, reDirectorList, reName ) );

              // Producer
            QRegExp reProducerList( "Продюсер(.*)<a name" );
            film.SetColumnData( FilmItem::ProducerColumn, RegExpTools::ParseList( str, reProducerList, reName, 10 ) ); // 10 first producers

              // Composer
            QRegExp reComposerList( "Композитор(.*)<a name" );
            film.SetColumnData( FilmItem::ComposerColumn, RegExpTools::ParseList( str, reComposerList, reName ) );

              // Starring
            QRegExp reStarringList( "Актер(.*)<a name" );
            reName = QRegExp( "class=\"info\"><div class=\"name\"><a href=\"/name/.*>(.*)</a>" );
            film.SetColumnData( FilmItem::StarringColumn, RegExpTools::ParseList( str, reStarringList, reName, 20 ) ); // 20 first actors
        }
        else
        {
              // Director
            QRegExp reDirectorList( "itemprop=\"director\">(.*)</td>" );
            film.SetColumnData( FilmItem::DirectorColumn, RegExpTools::ParseList( str, reDirectorList, reName ) );

              // Producer
            QRegExp reProducerList( "itemprop=\"producer\">(.*)</td>" );
            film.SetColumnData( FilmItem::ProducerColumn, RegExpTools::ParseList( str, reProducerList, reName ) );

              // Composer
            QRegExp reComposerList( "itemprop=\"musicBy\">(.*)</td>" );
            film.SetColumnData( FilmItem::ComposerColumn, RegExpTools::ParseList( str, reComposerList, reName ) );

              // Starring
            QRegExp reStarringList( "В главных ролях:</h4>(.*)</ul>" );
            film.SetColumnData( FilmItem::StarringColumn, RegExpTools::ParseList( str, reStarringList, reName ) );
        }

          // Poster

        if( AlexandraSettings::GetInstance()->GetParsersLoadBigPoster() )
        {
            QRegExp rePosterUrl( "openImgPopup\\('(.*)'\\)" );
            QString posterSubUrl = RegExpTools::ParseItem( str, rePosterUrl );

            if( !posterSubUrl.isEmpty() )
            {
                posterUrl = "http://www.kinopoisk.ru" + posterSubUrl;
            }
        }
        else
        {
            QRegExp rePosterUrl( "class=\"popupBigImage\".*><.*src=\"(.*)\" alt.*>" );
            posterUrl = QUrl( RegExpTools::ParseItem( str, rePosterUrl ) );
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

