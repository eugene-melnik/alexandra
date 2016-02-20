/*************************************************************************************************
 *                                                                                                *
 *  file: filmslistloader.cpp                                                                     *
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

#include "filmslistloader.h"
#include "tools/debug.h"
#include "version.h"

#include <QDataStream>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>


bool FilmsListLoader::Populate( FilmItem* rootItem, const QString& fileName )
{
    DebugPrintFunc( "FilmsListOldLoader::Populate", fileName );


//    QFile file( fileName );

//    if( file.open( QIODevice::ReadOnly ) )
//    {
//        QDataStream stream( &file );
//        stream.setVersion( QDataStream::Qt_5_3 );

//        DebugPrint( "Success!" );
//        DebugPrintFuncDone( "FilmsListOldLoader::Populate" );
//        return( true );
//        }
//    }

    DebugPrint( "Failed!" );
    DebugPrintFuncDone( "FilmsListOldLoader::Populate" );
    return( false );
}


bool FilmsListLoader::Save( FilmItem* rootItem, const QString& fileName )
{
    DebugPrintFunc( "FilmsListLoader::Save", fileName );

    QJsonArray filmsArray;

    for( int row = 0; row < rootItem->GetChildrenCount(); ++row )
    {
        FilmItem* film = rootItem->GetChild( row );
        filmsArray.append( FromFilmToJsonObject(film) );
    }

    QJsonObject rootObject;
    rootObject.insert( "films", filmsArray );
    QJsonDocument rootDocument( rootObject );
    QByteArray data = rootDocument.toJson( QJsonDocument::Compact );

    QFile file( fileName );

    if( file.open( QIODevice::WriteOnly ) || file.write(data) == -1 )
    {
        QDataStream stream( &file );
        stream.setVersion( QDataStream::Qt_5_3 );
        stream << Alexandra::databaseHeader;
        stream << Alexandra::databaseVersion;
        stream << data;
    }
    else
    {
        DebugPrint( "Saving failed!" );
        DebugPrintFuncDone( "FilmsListLoader::Save" );
        return( false );
    }

    DebugPrintFuncDone( "FilmsListLoader::Save" );
    return( true );
}


QJsonObject FilmsListLoader::FromFilmToJsonObject( FilmItem* film )
{
    QJsonObject filmObject;

    for( int column = 0; column < film->GetColumnCount(); ++column )
    {
        if( column == FilmItem::IsViewedColumn )
        {
              // Skip calculated values
            continue;
        }

        QString key = QString::number( column );
        QString value = film->GetColumnData( column ).toString();
        filmObject[ key ] = value;
    }

    filmObject[ "type" ] = film->GetFilmType();

    return( filmObject );
}

