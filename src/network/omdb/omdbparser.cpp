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

OmdbParser::OmdbParser()
{
    connect( &request, &NetworkRequest::DataLoaded, this, &OmdbParser::DataLoaded );
    connect( &request, &NetworkRequest::DataLoadError, this, [this] (const QString& e) { emit Error( e ); } );
    connect( &request, &NetworkRequest::Progress, this, [this] (quint64 received, quint64 total) { emit Progress( received, total ); } );
}

void OmdbParser::SearchFor( const QString& title, const QString& year )
{
    QUrl searchUrl;

    if( year.isEmpty() )
    {
        searchUrl = QString( "http://www.omdbapi.com/?t=%1&plot=full&r=%2" )
                            .arg( title )
                            .arg( "json" );
    }
    else
    {
        searchUrl = QString( "http://www.omdbapi.com/?t=%1&y=%2&plot=full&r=%3" )
                            .arg( title )
                            .arg( year )
                            .arg( "json" );
    }

    request.run( searchUrl );
}

void OmdbParser::SyncSearchFor( Film* filmSaveTo, QString* posterFileNameSaveTo,
                                const QString& title, const QString& year)
{
    QUrl searchUrl;

    if( year.isEmpty() )
    {
        searchUrl = QString( "http://www.omdbapi.com/?t=%1&plot=full&r=%2" )
                            .arg( title )
                            .arg( "json" );
    }
    else
    {
        searchUrl = QString( "http://www.omdbapi.com/?t=%1&y=%2&plot=full&r=%3" )
                            .arg( title )
                            .arg( year )
                            .arg( "json" );
    }

    QByteArray data = request.runSync( searchUrl );
    QString poster = Parse( data );

    if( filmSaveTo != nullptr )
    {
        // TODO: explain this
        QString title = filmSaveTo->GetTitle();
        QString fileName = filmSaveTo->GetFileName();

        filmSaveTo->SetNewData( f );

        filmSaveTo->SetTitle( title );
        filmSaveTo->SetFileName( fileName );
    }

    if( posterFileNameSaveTo != nullptr )
    {
        *posterFileNameSaveTo = poster;
    }
}

void OmdbParser::DataLoaded( const QByteArray& data )
{
    DebugPrintFuncA( "OmdbParser::DataLoaded", data.size() );
    Parse( data );
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
        f.SetOriginalTitle( json["Title"].toString() );
        f.SetYearFromStr(   json["Year"].toString() );
        f.SetCountry(       json["Country"].toString() );
        f.SetDirector(      json["Director"].toString() );
        f.SetGenre(         json["Genre"].toString() );
        f.SetScreenwriter(  json["Writer"].toString() );
        f.SetRatingFromStr( json["imdbRating"].toString() );
        f.SetStarring(      json["Actors"].toString() );
        f.SetDescription(   json["Plot"].toString() );

        QString posterUrl = json["Poster"].toString();

        if( posterUrl.length() > 5 )
        {
            DebugPrint( "Loading poster" );

            QByteArray& data = request.runSync( posterUrl );
            QString posterFileName( QDir::tempPath() + QString( "/tmpPoster%1" ).arg( rand() ) );
            QFile file( posterFileName );

            if( file.open( QIODevice::WriteOnly) && file.write( data ) )
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
