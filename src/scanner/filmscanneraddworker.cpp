/*************************************************************************************************
 *                                                                                                *
 *  file: filmscanneraddworker.cpp                                                                *
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

#include "alexandrasettings.h"
#include "filmscanneraddworker.h"
#include "tools/filesextensions.h"
#include "parsers/parsermanager.h"

#include <QDir>
#include <QFileInfo>
#include <QRegExp>

void FilmScannerAddWorker::run()
{
    AlexandraSettings* settings = AlexandraSettings::GetInstance();

    QList<Film> newFilms;
    ParserManager parser;
    int count = 0;

    for( const QString& fileName : foundedFilms )
    {
        Film film;
        film.SetId( Film::GetRandomHash() );
        QString title = Film::ClearTitle( QFileInfo( fileName ).completeBaseName() );
        film.SetFileName( fileName );
        film.SetTitle( title );

        QString posterFileName;
        QString newPosterFileName = settings->GetPostersDirPath() + "/" + film.GetPosterName();

        QRegExp regexp( "(185[0-9]|18[6-9][0-9]|19[0-9]{2}|200[0-9]|201[0-9])" ); // Years between 1850 and 2019
        regexp.indexIn( fileName );
        film.SetYearFromStr( regexp.cap(1) );

        // Search for a poster on the disk
        if( searchForPoster )
        {
            posterFileName = FilesExtensions().SearchForEponymousImage( fileName );
        }

        // Load information from online source
        if( loadInformation )
        {
            parser.Reset();
            parser.SetTitle( title );
            parser.SetParserId( ParserManager::Parser( settings->GetDefaultParserIndex() ) );

            if( film.GetYearStr().length() == 4 )
            {
                parser.SetYear( film.GetYearStr() );
            }

            if( posterFileName.isEmpty() )
            {
                parser.SetLoadPoster( true );
                parser.SearchSync( &film, &posterFileName );
            }
            else
            {
                parser.SetLoadPoster( false );
                parser.SearchSync( &film, nullptr );
            }
        }

        // Moving poster
        if( !posterFileName.isEmpty() )
        {
            if( SavePosterTo( posterFileName, newPosterFileName ) )
            {
                film.SetIsPosterExists( true );
            }
            else
            {
                film.SetIsPosterExists( false );
            }
        }

        // Adding film to the list
        newFilms.append( film );
        emit Progress( ++count, foundedFilms.size() );
    }

    emit FilmsCreated( newFilms );
}

bool FilmScannerAddWorker::SavePosterTo( const QString& sourceName, const QString& destinationName )
{
    AlexandraSettings* settings = AlexandraSettings::GetInstance();

    QString postersDir = settings->GetPostersDirPath();
    int newHeight = settings->GetScalePosterToHeight();

    // Creating posters' directory if not exists
    if( !QDir().exists( postersDir ) )
    {
        QDir().mkdir( postersDir );
    }

    QPixmap p( sourceName );

    // Scale to height
    if( newHeight != 0 && newHeight < p.height() )
    {
        p = p.scaledToHeight( newHeight, Qt::SmoothTransformation );
    }

    // Move to posters' folder
    QString format = settings->GetPosterSavingFormat();
    int quality = settings->GetPosterSavingQuality();

    return( p.save( destinationName, format.toUtf8(), quality ) );
}
