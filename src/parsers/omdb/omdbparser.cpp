/*************************************************************************************************
 *                                                                                                *
 *  file: omdbparser.cpp                                                                          *
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

#include "omdbparser.h"
#include "tools/debug.h"

#include <QJsonDocument>
#include <QJsonObject>


OmdbParser::OmdbParser() : AbstractParser()
{
    DebugPrintFunc( "OmdbParser::OmdbParser" );
    searchUrlWithYear = QString( "http://www.omdbapi.com/?t=%1&y=%2&plot=full&r=json" );
    searchUrl = QString( "http://www.omdbapi.com/?t=%1&plot=full&r=json" );

      // Strings for localization mechanism of Qt
      // which may be returned by server
    tr("Movie not found!");
}


QUrl OmdbParser::Parse( const QByteArray& data )
{
    DebugPrintFunc( "OmdbParser::Parse", data.size() );

    QJsonObject json = QJsonDocument::fromJson( data ).object();
    QUrl posterUrl;

    if( json["Response"].toString() == "True" )
    {
        film.SetColumnData( FilmItem::TitleColumn, json["Title"].toString() );
        film.SetColumnData( FilmItem::OriginalTitleColumn, json["Title"].toString() );
        film.SetColumnData( FilmItem::YearColumn, json["Year"].toInt() );

        if( json["Country"].toString() != QLatin1String( "N/A" ) )
            film.SetColumnData( FilmItem::CountryColumn, json["Country"].toString() );

        if( json["Director"].toString() != QLatin1String( "N/A" ) )
            film.SetColumnData( FilmItem::DirectorColumn, json["Director"].toString() );

        if( json["Genre"].toString() != QLatin1String( "N/A" ) )
            film.SetColumnData( FilmItem::GenreColumn, json["Genre"].toString() );

        if( json["Writer"].toString() != QLatin1String( "N/A" ) )
            film.SetColumnData( FilmItem::ScreenwriterColumn, json["Writer"].toString() );

        if( json["imdbRating"].toString() != QLatin1String( "N/A" ) )
            film.SetColumnData( FilmItem::RatingColumn, json["imdbRating"].toDouble() );

        if( json["Actors"].toString() != QLatin1String( "N/A" ) )
            film.SetColumnData( FilmItem::StarringColumn, json["Actors"].toString() );

        if( json["Plot"].toString() != QLatin1String( "N/A" ) )
            film.SetColumnData( FilmItem::DescriptionColumn, json["Plot"].toString() );

        if( json["Poster"].toString() != QLatin1String( "N/A" ) )
            posterUrl = json["Poster"].toString();

        emit Loaded( film, posterUrl );
    }
    else
    {
        emit Error( tr( json["Error"].toString().toUtf8() ) );
    }

    return( posterUrl );
}

