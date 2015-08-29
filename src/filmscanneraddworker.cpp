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

#include "filmscanneraddworker.h"
#include "filesextensions.h"

#include <QDir>
#include <QFileInfo>

void FilmScannerAddWorker::run()
{
    QList<Film> newFilms;

    for( const QString& fileName : foundedFilms )
    {
        Film f;
        f.SetId( Film::GetRandomHash() );
        f.SetFileName( fileName );
        f.SetTitle( QFileInfo( fileName ).completeBaseName().replace( "_", " " ) );

        // Search for a poster
        if( searchForPoster )
        {
            QString posterFileName = FilesExtensions().SearchForEponymousImage( fileName );

            if( !posterFileName.isEmpty() )
            {
                f.SetIsPosterExists( true );

                QString postersDir = settings->GetPostersDirPath();
                int newHeight = settings->GetScalePosterToHeight();

                if( QFileInfo( posterFileName ).absolutePath() != postersDir )
                {
                    // Creating posters' directory if not exists
                    if( !QDir().exists( postersDir ) )
                    {
                        QDir().mkdir( postersDir );
                    }

                    QPixmap p( posterFileName );

                    // Scale to height
                    if( newHeight != 0 && newHeight < p.height() )
                    {
                        p = p.scaledToHeight( newHeight, Qt::SmoothTransformation );
                    }

                    // Move to posters' folder
                    QString newPosterFileName = postersDir + "/" + f.GetPosterName();
                    std::string format = settings->GetPosterSavingFormat().toStdString();
                    int quality = settings->GetPosterSavingQuality();

                    if( !p.save( newPosterFileName, format.c_str(), quality ) )
                    {
                        f.SetIsPosterExists( false );
                    }
                }
            }
        }

        // Adding
        newFilms.append( f );
    }

    emit FilmsCreated( newFilms );
}
