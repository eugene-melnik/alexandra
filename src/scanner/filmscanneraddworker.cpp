/*************************************************************************************************
 *                                                                                                *
 *  file: filmscanneraddworker.cpp                                                                *
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

#include "filmscanneraddworker.h"
#include "tools/filesextensions.h"

#include <QDir>
#include <QFileInfo>


FilmScannerAddWorker::FilmScannerAddWorker() : QThread(),
    settings( AlexandraSettings::GetInstance() )
{
    qRegisterMetaType<FilmItemList>( "FilmItemList" );
}


void FilmScannerAddWorker::run()
{

    for( const QString& fileName : this->foundFilms )
    {
        if( isCanceled ) break;

        int year;
        QString title = FilmItem::GetClearedTitle( QFileInfo(fileName).completeBaseName(), &year );

        FilmItem* film = new FilmItem();
        film->SetColumnData( FilmItem::FileNameColumn, fileName );
        film->SetColumnData( FilmItem::TitleColumn, title );
        film->SetColumnData( FilmItem::YearColumn, year );

        QString posterFileName;

          // Search for a poster on the disk
        if( searchForPoster )
        {
            posterFileName = FilesExtensions::SearchForEponymousImage( fileName );
        }

          // Load information from online source
        if( loadInformation )
        {
            parser.Reset();
            parser.SetTitle( title );
            parser.SetParserId( ParserManager::Parser( settings->GetDefaultParserIndex() ) );

            if( year > 0 )
            {
                parser.SetYear( year );
            }

            if( posterFileName.isEmpty() )
            {
                parser.SetLoadPoster( true );
                parser.SearchSync( film, &posterFileName );
            }
            else
            {
                parser.SetLoadPoster( false );
                parser.SearchSync( film, nullptr );
            }
        }

          // Moving poster
        if( !posterFileName.isEmpty() )
        {
            if( SavePosterTo( posterFileName, film->GetPosterFilePath() ) )
            {
                film->SetIsPosterExists( FilmItem::Exists );
            }
            else
            {
                film->SetIsPosterExists( FilmItem::NotExists );
            }
        }

          // Adding film to the list
        film->SetIsFileExists( FilmItem::Exists );
        emit FilmCreated( film );
    }
}


bool FilmScannerAddWorker::SavePosterTo( QString sourceName, QString destinationName )
{
    QString postersDir = settings->GetPostersDirPath();
    int newHeight = settings->GetScalePosterToHeight();

      // Creating posters' directory if not exists
    if( !QFile::exists( postersDir ) )
    {
        QDir().mkdir( postersDir );
    }

    QPixmap pixmap( sourceName );

      // Scale to height
    if( newHeight != 0 && newHeight < pixmap.height() )
    {
        pixmap = pixmap.scaledToHeight( newHeight, Qt::SmoothTransformation );
    }

      // Move to posters' folder
    QString format = settings->GetPosterSavingFormat();
    int quality = settings->GetPosterSavingQuality();

    return( pixmap.save( destinationName, format.toUtf8(), quality ) );
}

