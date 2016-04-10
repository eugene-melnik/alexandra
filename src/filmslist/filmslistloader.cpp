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


bool FilmsListLoader::Populate( FilmItem* rootItem, QString fileName )
{
    DebugPrintFunc( "FilmsListLoader::Populate", fileName );

    QFile file( fileName );

    if( file.open(QIODevice::ReadOnly) )
    {
        QDataStream stream( &file );
        stream.setVersion( QDataStream::Qt_5_3 );

        QString databaseHeader; quint8 databaseVersion;
        stream >> databaseHeader >> databaseVersion;

        QByteArray data;
        stream >> data;
        file.close();

        if( !data.isEmpty() )
        {
            QJsonDocument rootDocument = QJsonDocument::fromJson(data);
            QJsonObject rootObject = rootDocument.object();
            QJsonArray filmsArray = rootObject["films"].toArray();

            for( int i = 0; i < filmsArray.count(); ++i )
            {
                rootItem->AppendChild( FromJsonObjectToFilm( filmsArray.at(i).toObject() ) );
            }
        }

        DebugPrint( "Success!" );
        DebugPrintFuncDone( "FilmsListLoader::Populate" );
        return( true );
    }

    DebugPrint( "Failed!" );
    DebugPrintFuncDone( "FilmsListLoader::Populate" );
    return( false );
}


bool FilmsListLoader::Save( FilmItem* rootItem, QString fileName, QString* errorString )
{
    DebugPrintFunc( "FilmsListLoader::Save", fileName );

    QJsonArray filmsArray;

    for( int row = 0; row < rootItem->GetChildrenCount(); ++row )
    {
        FilmItem* film = rootItem->GetChild( row );
        filmsArray.append( FromFilmToJsonObject(film) );
    }

    QJsonObject rootObject;
    rootObject["films"] = filmsArray;
    QJsonDocument rootDocument( rootObject );

    #ifdef QT_DEBUG
        QByteArray data = rootDocument.toJson( QJsonDocument::Indented );
    #else
        QByteArray data = rootDocument.toJson( QJsonDocument::Compact );
    #endif

    QFile file( fileName );

    if( file.open( QIODevice::WriteOnly ) )
    {
        QDataStream stream( &file );
        stream.setVersion( QDataStream::Qt_5_3 );
        stream << Alexandra::databaseHeader;
        stream << Alexandra::databaseVersion;
        stream << data;
        file.close();
    }
    else
    {
        DebugPrint( "Saving failed!" );
        DebugPrintFuncDone( "FilmsListLoader::Save" );

        if (errorString != nullptr)
        {
            *errorString = file.errorString();
        }

        return( false );
    }

    DebugPrintFuncDone( "FilmsListLoader::Save" );
    return( true );
}


bool FilmsListLoader::CreateEmptyDatabase( QString fileName )
{
    QFile file( fileName );

    if( file.open(QIODevice::WriteOnly) )
    {
        QDataStream stream( &file );
        stream.setVersion( QDataStream::Qt_5_3 );
        stream << Alexandra::databaseHeader;
        stream << Alexandra::databaseVersion;
        return( true );
    }

    return( false );
}


FilmItem* FilmsListLoader::FromJsonObjectToFilm( QJsonObject object )
{
    FilmItem* film = new FilmItem();

    for( int column = 0; column < FilmItem::AllColumnCount; ++column )
    {
        film->SetColumnData( column, object[ QString::number(column) ].toString() );
    }

    int viewsCount = film->GetColumnData( FilmItem::ViewsCountColumn ).toInt();
    film->SetColumnData( FilmItem::IsViewedColumn, (viewsCount > 0) );

    return( film );
}


QJsonObject FilmsListLoader::FromFilmToJsonObject( FilmItem* film )
{
    QJsonObject filmObject;
    filmObject["type"] = film->GetFilmType();

    for( int column = 0; column < FilmItem::AllColumnCount; ++column )
    {
        if( column == FilmItem::IsViewedColumn )
        {
              // Skip calculated values
            continue;
        }

        QString key = QString::number( column );
        QString value = film->GetColumnData( column ).toString();

        if( !value.isEmpty() )
        {
            filmObject[ key ] = value;
        }
    }

    return( filmObject );
}

