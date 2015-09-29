/*************************************************************************************************
 *                                                                                                *
 *  file: omdbparser.cpp                                                                          *
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

#include "omdbparser.h"
#include "debug.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

void OmdbParser::SearchFor( const QString& title, const QString& year )
{
    searchUrlWithYear = QString( "http://www.omdbapi.com/?t=%1&y=%2&plot=full&r=json" );
    searchUrl = QString( "http://www.omdbapi.com/?t=%1&plot=full&r=json" );
    AbstractParser::SearchFor( title, year );
}

void OmdbParser::SyncSearchFor( Film* filmSaveTo, QString* posterFileNameSaveTo,
                                const QString& title, const QString& year)
{
    searchUrlWithYear = QString( "http://www.omdbapi.com/?t=%1&y=%2&plot=full&r=json" );
    searchUrl = QString( "http://www.omdbapi.com/?t=%1&plot=full&r=json" );
    AbstractParser::SyncSearchFor( filmSaveTo, posterFileNameSaveTo, title, year );
}

QString OmdbParser::Parse( const QByteArray& data )
{
    DebugPrintFuncA( "OmdbParser::Parse", data.size() );

    QJsonObject json = QJsonDocument::fromJson( data ).object();

    for( const QString& s : json.keys() )
    {
        DebugPrint( "JSON key: " + s + " = " + json[s].toString() );
    }

    if( json["Response"].toString() == "True" )
    {
        f.SetTitle( json["Title"].toString() );
        f.SetOriginalTitle( json["Title"].toString() );
        f.SetYearFromStr(   json["Year"].toString() );

        if( json["Country"].toString() != QLatin1String( "N/A" ) )
            f.SetCountry( json["Country"].toString() );

        if( json["Director"].toString() != QLatin1String( "N/A" ) )
            f.SetDirector( json["Director"].toString() );

        if( json["Genre"].toString() != QLatin1String( "N/A" ) )
            f.SetGenre( json["Genre"].toString() );

        if( json["Writer"].toString() != QLatin1String( "N/A" ) )
            f.SetScreenwriter( json["Writer"].toString() );

        if( json["imdbRating"].toString() != QLatin1String( "N/A" ) )
            f.SetRatingFromStr( json["imdbRating"].toString() );

        if( json["Actors"].toString() != QLatin1String( "N/A" ) )
            f.SetStarring( json["Actors"].toString() );

        if( json["Plot"].toString() != QLatin1String( "N/A" ) )
            f.SetDescription( json["Plot"].toString() );

        QString posterUrl = json["Poster"].toString();

        if( posterUrl.length() > 5 )
        {
            DebugPrint( "Loading poster" );

            QByteArray& data = request.runSync( posterUrl );
            QString posterFileName( QDir::tempPath() + QString( "/tmpPoster%1" ).arg( rand() ) );
            QFile file( posterFileName );

            if( file.open( QIODevice::WriteOnly ) && file.write( data ) )
            {
                file.close();
                emit Loaded( f, posterFileName );
                return( posterFileName );
            }
            else
            {
                Error( file.errorString() );
            }
        }
        else
        {
            emit Loaded( f, QString() );
        }
    }
    else
    {
        Error( json["Error"].toString() );
    }

    return( QString() );
}
