/*************************************************************************************************
 *                                                                                                *
 *  file: parsermanager.cpp                                                                       *
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

#include "parsermanager.h"
#include "abstractparser.h"
#include "kinopoisk/kinopoiskparser.h"
#include "imdb/imdbparser.h"
#include "omdb/omdbparser.h"
#include "debug.h"

#include <QFile>
#include <QLocale>

ParserManager::ParserManager( ParserManager::Parser p ) : selectedParserId( p )
{
    parsers.insert( Auto, tr( "<Auto>" ) );
    parsers.insert( OMDB, "OMDB (http://www.omdbapi.com/)" );
//    parsers.insert( IMDB, "IMDB (http://www.imdb.com/)" );
    parsers.insert( Kinopoisk, "КиноПоиск (http://www.kinopoisk.ru/)" );
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

    if( title.isEmpty() ) return;

    CreateParser();
    connect( currentParser, SIGNAL( Progress(quint64,quint64) ), this, SLOT( ProgressChanged(quint64,quint64) ) );
    connect( currentParser, SIGNAL( Loaded(const Film&, const QUrl&) ), this, SLOT( InformationLoaded(const Film&, const QUrl&) ) );
    connect( currentParser, SIGNAL( Error(const QString&) ), this, SLOT( InformationLoadError(const QString&) ) );

    AbstractParser* cp = dynamic_cast<AbstractParser*>( currentParser );
    cp->SearchFor( title, year );
}

void ParserManager::SearchAsync( Film* filmSaveTo, QString* posterFileNameSaveTo )
{
    DebugPrintFunc( "ParserManager::SearchAsync" );

    if( title.isEmpty() ) return;

    CreateParser();
    connect( currentParser, SIGNAL( Progress(quint64,quint64) ), this, SLOT( ProgressChanged(quint64,quint64) ) );

    QUrl posterUrl;
    AbstractParser* cp = dynamic_cast<AbstractParser*>( currentParser );
    cp->SyncSearchFor( filmSaveTo, &posterUrl, title, year );

    if( posterFileNameSaveTo != nullptr )
    {
        posterFileNameSaveTo->clear();

        if( !posterUrl.isEmpty() && SavePoster( posterUrl, stdPosterFileName ) )
        {
            *posterFileNameSaveTo = stdPosterFileName;
        }
    }
}

void ParserManager::InformationLoaded( const Film& f, const QUrl& posterUrl )
{
    DebugPrintFuncA( "ParserManager::InformationLoaded", f.GetOriginalTitle() + ", " + stdPosterFileName );

    if( loadPoster )
    {
        if( SavePoster( posterUrl, stdPosterFileName ) )
        {
            emit Loaded( f, stdPosterFileName );
            return;
        }
        else
        {
            emit Error( tr( "Failed to save the poster to \"%1\"!" ).arg( stdPosterFileName ) );
        }
    }

    emit Loaded( f, QString() );
}

void ParserManager::InformationLoadError( const QString& e )
{
    DebugPrintFuncA( "ParserManager::InformationLoadError", e );
    emit Error( e );
}

void ParserManager::CreateParser()
{
    DebugPrintFunc( "ParserManager::CreateParser" );
    DebugPrint( "Parser: " + QString::number( selectedParserId ) );

    if( currentParser != nullptr )
    {
        delete currentParser;
    }

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

        default : // Parser::Auto
        {
            // TODO: more logic
            QStringList kyr = QStringList { "ru", "uk", "be" };

            if( kyr.contains( QLocale::system().name().left(2) ) )
            {
                currentParser = new KinopoiskParser();
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
    DebugPrint( "Loading poster..." );

    QByteArray& posterData = NetworkRequest().runSync( posterUrl );
    QFile file( posterFileName );

    if( file.open( QIODevice::WriteOnly ) && file.write( posterData ) )
    {
        file.close();
        return( true );
    }
    else
    {
        return( false );
    }
}
