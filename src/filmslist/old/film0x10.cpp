/*************************************************************************************************
 *                                                                                                *
 *  file: film.cpp                                                                                *
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

#include "film0x10.h"
#include "tools/debug.h"


Film010::Film010() : viewsCounter( 0 ), isPosterExists( false ), isViewed( false ), isFavourite( false )
{}


QString Film010::GetBudgetStr() const
{
    QString result;

    if( budget > 1 )
    {
        result = QString( "$%L1" ).arg( budget, 0, 'f', 0 );
    }

    return( result );
}


QString Film010::GetPosterName() const
{
    if( GetIsPosterExists() )
    {
        return( GetId() );
    }
    else
    {
        return( QString() );
    }
}


QDataStream& operator >> ( QDataStream& in, Film010& film )
{
    QString additionalData;

    in >> film.id
       >> film.section
       >> film.fileName
       >> film.title
       >> film.originalTitle
       >> film.tagline
       >> film.genre
       >> film.country
       >> film.year
       >> film.director
       >> film.producer
       >> film.starring
       >> film.budget
       >> film.description
       >> film.rating
       >> film.ageRestrictions
       >> film.tags
       >> additionalData
       >> film.viewsCounter
       >> film.isPosterExists
       >> film.isViewed
       >> film.isFavourite;

      // Additional fields
    int version; // unused
    QByteArray dataArray = QByteArray::fromHex( additionalData.toLatin1() );
    QDataStream stream( dataArray );
    stream >> version >> film.screenwriter >> film.composer;

    return( in );
}

