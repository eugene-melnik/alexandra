/*************************************************************************************************
 *                                                                                                *
 *  file: filmslistoldloader.cpp                                                                  *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2017 Eugene Melnik <eugene@melnik.solutions>                               *
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

#include "filmslistoldloader.h"
#include "film0x10.h"
#include "tools/filesextensions.h"
#include "tools/debug.h"

#include <QDataStream>
#include <QFile>
#include <QDir>
#include <QList>
#include <QVariant>


bool FilmsListOldLoader::Populate( FilmItem* rootItem, const QString& fileName )
{
    DebugPrintFunc( "FilmsListOldLoader::Populate", fileName );

    QFile file( fileName );

    if( file.open(QIODevice::ReadOnly) )
    {
        AlexandraSettings* settings = AlexandraSettings::GetInstance();

        QDataStream stream( &file );
        QString header;
        quint8 version;
        stream >> header >> version;

        if( version == OldVersion10 )
        {
            QList<Film010> films;
            stream >> films;

            for( Film010& film : films )
            {
                // Views counter
                int viewsCount = film.GetViewsCounter();

                if( film.GetIsViewed() && viewsCount == 0 )
                {
                    viewsCount = 1;
                }

                bool isViewed = (viewsCount > 0) ? true : false;

                // Poster files renaming (for very old databases)
                QString posterFilename = film.GetPosterName();

                if( !posterFilename.isEmpty() )
                {
                    QString posterFilepath = settings->GetPostersDirPath() + "/" + posterFilename;
                    QFile posterFile( posterFilepath );

                    if( !posterFile.exists() )
                    {
                        QString maybeOldPoster = FilesExtensions::SearchForEponymousImage(posterFilepath);

                        if ( !maybeOldPoster.isEmpty() )
                        {
                            QFileInfo fileinfo( maybeOldPoster );
                            QDir postersDir( fileinfo.absolutePath() );
                            postersDir.rename( fileinfo.fileName(), posterFilename );
                        }
                    }
                }

                // Filling data
                QList<QVariant> data;

                data << film.GetTitle()
                     << film.GetOriginalTitle()
                     << film.GetTagline()
                     << film.GetYear()
                     << film.GetGenre()
                     << film.GetCountry()
                     << film.GetDirector()
                     << film.GetProducer()
                     << film.GetScreenwriter()
                     << film.GetComposer()
                     << film.GetBudget()
                     << film.GetRating()
                     << isViewed
                     << film.GetIsFavourite()
                     << viewsCount
                     << film.GetStarring()
                     << film.GetDescription()
                     << film.GetTags()
                     << film.GetFileName()
                     << film.GetId(); // PosterName

                FilmItem* item = new FilmItem( data, rootItem );
                rootItem->AppendChild( item );
            }

            DebugPrint( "Success!" );
            DebugPrintFuncDone( "FilmsListOldLoader::Populate" );
            return( true );
        }
    }

    DebugPrint( "Failed!" );
    DebugPrintFuncDone( "FilmsListOldLoader::Populate" );
    return( false );
}

