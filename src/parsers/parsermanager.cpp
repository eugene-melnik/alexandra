/*************************************************************************************************
 *                                                                                                *
 *  file: parsermanager.cpp                                                                       *
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

#include "parsermanager.h"
#include "abstractparser.h"
#include "kinopoisk/kinopoiskparser.h"
#include "kinoteatr/kinoteatrparser.h"
#include "imdb/imdbparser.h"
#include "omdb/omdbparser.h"
#include "tools/debug.h"

#include <QFile>
#include <QLocale>


ParserManager::ParserManager( ParserManager::Parser p ) : selectedParserId( p )
{
    parsers.insert( Auto,       tr( "<Auto>" ) );
    parsers.insert( IMDB,       "IMDB (http://www.imdb.com/)" );
    parsers.insert( Kinopoisk,  "КиноПоиск (http://www.kinopoisk.ru/)" );
    parsers.insert( Kinoteatr,  "Кіно-Театр (http://kino-teatr.ua/)" );
    parsers.insert( OMDB,       "OMDB (http://www.omdbapi.com/)" );
}


void ParserManager::Reset()
{
    selectedParserId = Auto;
    title.clear();
    year.clear();
    loadPoster = true;
}


void ParserManager::Search()
{
    DebugPrintFunc( "ParserManager::Search" );
    time.start();

    if( title.isEmpty() ) return;

    CreateParser();
    connect( currentParser, SIGNAL( Progress(quint64,quint64) ), this, SLOT( ProgressChanged(quint64,quint64) ) );
    connect( currentParser, SIGNAL( Loaded(const FilmItem&, const QUrl&) ), this, SLOT( InformationLoaded(FilmItem, const QUrl&) ) );
    connect( currentParser, SIGNAL( Loaded(const FilmItem&, const QUrl&) ), currentParser, SLOT( deleteLater() ) );
    connect( currentParser, SIGNAL( Error(const QString&) ), this, SLOT( InformationLoadError(const QString&) ) );
    connect( currentParser, SIGNAL( Error(const QString&) ), currentParser, SLOT( deleteLater() ) );

    AbstractParser* cp = dynamic_cast<AbstractParser*>( currentParser );
    cp->SearchFor( title, year );
}


void ParserManager::SearchSync( FilmItem* filmSaveTo, QString* posterFileNameSaveTo )
{
    DebugPrintFunc( "ParserManager::SearchAsync" );
    time.start();

    if( title.isEmpty() )
    {
        emit Error( tr( "Nothing to search." ) );
        return;
    }

    CreateParser();
    connect( currentParser, SIGNAL( Progress(quint64,quint64) ), this, SLOT( ProgressChanged(quint64,quint64) ) );

    QUrl posterUrl;
    AbstractParser* cp = dynamic_cast<AbstractParser*>( currentParser );
    cp->SyncSearchFor( filmSaveTo, &posterUrl, title, year );

    if( loadPoster && posterFileNameSaveTo != nullptr )
    {
        posterFileNameSaveTo->clear();

        if( !posterUrl.isEmpty() && SavePoster( posterUrl, stdPosterFileName ) )
        {
            *posterFileNameSaveTo = stdPosterFileName;
        }
    }

    cp->deleteLater();
}


void ParserManager::InformationLoaded( FilmItem film, const QUrl& posterUrl )
{
    DebugPrintFunc( "ParserManager::InformationLoaded", film.GetColumnData( FilmItem::OriginalTitleColumn ).toString() );
    DebugPrint( QString( "Loded in %1 ms" ).arg( time.elapsed() ) );

    QString poster;

    if( loadPoster && SavePoster( posterUrl, stdPosterFileName ) )
    {
        film.SetIsPosterExists( FilmItem::Exists );
        poster = stdPosterFileName;
    }

    emit Loaded( film, poster );
}


void ParserManager::InformationLoadError( const QString& errorString )
{
    DebugPrintFunc( "ParserManager::InformationLoadError", errorString );
    DebugPrint( QString( "Loded in %1 ms" ).arg( time.elapsed() ) );
    emit Error( errorString );
}


void ParserManager::CreateParser()
{
    DebugPrintFunc( "ParserManager::CreateParser" );
    DebugPrint( "Parser: " + parsers.value( selectedParserId ) );

    switch( selectedParserId )
    {
        case OMDB :
        {
            currentParser = new OmdbParser();
            break;
        }

        case Kinopoisk :
        {
            currentParser = new KinopoiskParser();
            break;
        }

        case Kinoteatr :
        {
            currentParser = new KinoteatrParser();
            break;
        }

        case IMDB :
        {
            currentParser = new ImdbParser();
            break;
        }

        default : // Parser::Auto
        {
            // TODO: more logic

            if( QLocale::system().name().left(2) == "ru" )
            {
                currentParser = new KinopoiskParser();
            }
            else if( QLocale::system().name().left(2) == "uk" )
            {
                currentParser = new KinoteatrParser();
            }
            else
            {
                currentParser = new OmdbParser();
            }

            break;
        }
    }
}


bool ParserManager::SavePoster( QUrl posterUrl, QString posterFileName )
{
    DebugPrintFunc( "ParserManager::SavePoster" );

    if( !posterUrl.isEmpty() )
    {
        DebugPrint( "Url:  " + posterUrl.toString() );
        DebugPrint( "File: " + posterFileName );

        QByteArray posterData = NetworkRequest().runSync( posterUrl );
        QFile file( posterFileName );

        if( file.open(QIODevice::WriteOnly) && file.write(posterData) )
        {
            DebugPrintFuncDone( "ParserManager::SavePoster" );
            return( true );
        }
        else
        {
            DebugPrint( QString( "Error! %1" ).arg( file.errorString() ) );
        }
    }

    return( false );
}

