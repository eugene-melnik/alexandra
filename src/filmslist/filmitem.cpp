/*************************************************************************************************
 *                                                                                                *
 *  file: filmitem.cpp                                                                            *
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

#include "filmitem.h"


FilmItem::FilmItem() : filmId( GetRandomHash() )
{
    int i = ColumnCount;
    columnsData.reserve( i );

    while( i-- > 0 )
    {
        columnsData.append( QVariant() );
    }
}

FilmItem& FilmItem::operator = ( const FilmItem& item )
{
    parentItem = item.parentItem;
    filmId = item.filmId;
    filmType = item.filmType;
    isFileExists = item.isFileExists;
    isPosterExists = item.isPosterExists;

    columnsData.clear();
    columnsData = item.columnsData;
    RemoveChildren();

    for( FilmItem* film : childItems )
    {
        childItems.append( new FilmItem( *film ) );
    }

    return( *this );
}


void FilmItem::AppendChild( FilmItem* item )
{
    if( item->GetParent() == nullptr )
    {
        item->SetParent( this );
    }

    childItems.append( item );
}


void FilmItem::RemoveChildren()
{
    qDeleteAll( childItems );
    childItems.clear();
}


QString FilmItem::GetRandomHash()
{
    QByteArray hash = QCryptographicHash::hash( QByteArray::number( qrand() ), QCryptographicHash::Sha1 );
    return( QString( hash.toHex() ) );
}


QString FilmItem::ClearTitle( const QString& title )
{
    QString newTitle = title;

      // List of words (of course not all), which may be present
      // in the file name, but are not part of the title
    QStringList wordsToRemove =
    {
        "DHT-Movies",  "TheaterRip",  "BlueBird",  "HDTV720",  "HDTVRip",  "NeroAVC",  "UNRATED",  "BluRay",
        "DVDRip",      "HDclub",      "HDRip",     "WEB-DL",   "1080p",    "BDRip",    "720p",     "H264",
        "HDTV",        "HiDt",        "x264",      "AC3",      "AVC",      "AVO",      "DTS",      "Eng",
        "JRG",         "MKV",         "MVO",       "Fra",      "Rus",      "Ukr",      "HD"
    };

    for( const QString& word : wordsToRemove )
    {
        newTitle.replace( word, "", Qt::CaseInsensitive );
    }

    QStringList regexpsToRemove =
    {
        "(185[0-9]|18[6-9][0-9]|19[0-9]{2}|200[0-9]|201[0-9])", // Years between 1850 and 2019
        "\\[(.+)\\]|\\((.+)\\)",                                // Round and square brackets with the contents
        "\\(\\)|\\[\\]",                                        // Round and square brackets whithout contents
        "\\.|_|\\-",                                            // Dots, underscores and dashes (hyphens)
        "[0-9][xX]",                                            // Phrases like "NxSomething", where N is a number
        " {1,}",                                                // Multiple spaces
    };

    for( const QString& regexp : regexpsToRemove )
    {
        newTitle.replace( QRegExp(regexp), " " );
    }

      // Spaces at begin and end of the string
    newTitle = newTitle.trimmed();

    if( newTitle.length() > 1 )
    {
        return( newTitle );
    }
    else
    {
        return( title );
    }
}
