/*************************************************************************************************
 *                                                                                                *
 *  file: filmslistoldloader.cpp                                                                  *
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

#include "filmslistoldloader.h"
#include "film0x10.h"
#include "tools/debug.h"

#include <QDataStream>
#include <QFile>
#include <QList>
#include <QVariant>


bool FilmsListOldLoader::Populate( FilmItem* rootItem, const QString& fileName )
{
    DebugPrintFuncA( "FilmsListOldLoader::Populate", fileName );
    QFile file( fileName );

    if( file.open( QIODevice::ReadOnly ) )
    {
        QDataStream stream( &file );
        QString header;
        quint8 version;
        stream >> header >> version;

        if( version == OldVersion )
        {
            QList<Film010> films;
            stream >> films;

            for( Film010& film : films )
            {
                QList<QVariant> data;

                data << film.GetTitle()
                     << film.GetOriginalTitle()
                     << film.GetTagline()
                     << film.GetYearStr()
                     << film.GetGenre()
                     << film.GetCountry()
                     << film.GetDirector()
                     << film.GetProducer()
                     << film.GetScreenwriter()
                     << film.GetComposer()
                     << film.GetBudgetStr()
                     << film.GetRatingStr()
                     << film.GetIsViewedSign()
                     << film.GetIsFavouriteSign()
                     << film.GetViewsCounter()
                     << film.GetStarring()
                     << film.GetDescription()
                     << film.GetTags()
                     << film.GetFileName()
                     << film.GetPosterName();

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
