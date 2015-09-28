/*************************************************************************************************
 *                                                                                                *
 *  file: film.cpp                                                                                *
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

#include "film.h"
#include "debug.h"

#include <QByteArray>
#include <QCryptographicHash>
#include <QDataStream>

Film::Film()
    : viewsCounter( 0 ), isPosterExists( false ), isViewed( false ), isFavourite( false )
{ }

QString Film::GetBudgetStr() const
{
    QString result = "";

    if( budget > 1 ) // double has no zero
    {
        result = "$" + QString::number( budget, 'f', 0 );
    }

    return( result );
}

QDataStream& operator << ( QDataStream& out, const Film& f )
{
    // Additional fields
    QByteArray dataArray;
    QDataStream stream( &dataArray, QIODevice::WriteOnly );

    stream << f.additionalDataVersion // Unused yet
           << f.screenwriter
           << f.composer;

    QString additionalData = dataArray.toHex();

    // Write
    out << f.id
        << f.section
        << f.fileName
        << f.title
        << f.originalTitle
        << f.tagline
        << f.genre
        << f.country
        << f.year
        << f.director
        << f.producer
        << f.starring
        << f.budget
        << f.description
        << f.rating
        << f.ageRestrictions
        << f.tags
        << additionalData
        << f.viewsCounter
        << f.isPosterExists
        << f.isViewed
        << f.isFavourite;

    return( out );
}

QDataStream& operator >> ( QDataStream& in, Film& f )
{
    QString additionalData;

    // Read
    in >> f.id
       >> f.section
       >> f.fileName
       >> f.title
       >> f.originalTitle
       >> f.tagline
       >> f.genre
       >> f.country
       >> f.year
       >> f.director
       >> f.producer
       >> f.starring
       >> f.budget
       >> f.description
       >> f.rating
       >> f.ageRestrictions
       >> f.tags
       >> additionalData
       >> f.viewsCounter
       >> f.isPosterExists
       >> f.isViewed
       >> f.isFavourite;

    // Additional fields
    QByteArray dataArray = QByteArray::fromHex( additionalData.toLatin1() );
    QDataStream stream( dataArray );

    int version;
    stream >> version // unused yet
           >> f.screenwriter
           >> f.composer;

    return( in );
}

#ifdef QT_DEBUG
QString Film::DebugGetAllFields() const
{
    QString res = "\n-= Film Debug Info =-\n";

    res += QString( "id: \"%1\"\n" ).arg( id );
    res += QString( "section: \"%1\"\n" ).arg( section );
    res += QString( "fileName: \"%1\"\n" ).arg( fileName );
    res += QString( "title: \"%1\"\n" ).arg( title );
    res += QString( "originalTitle: \"%1\"\n" ).arg( originalTitle );
    res += QString( "tagline: \"%1\"\n" ).arg( tagline );
    res += QString( "genre: \"%1\"\n" ).arg( genre );
    res += QString( "country: \"%1\"\n" ).arg( country );
    res += QString( "year: \"%1\"\n" ).arg( year );
    res += QString( "budget: \"%1\"\n" ).arg( budget );
    res += QString( "director: \"%1\"\n" ).arg( director );
    res += QString( "producer: \"%1\"\n" ).arg( producer );
    res += QString( "starring: \"%1\"\n" ).arg( starring );
    res += QString( "budget: \"%1\"\n" ).arg( budget );
    res += QString( "description: \"%1\"\n" ).arg( description );
    res += QString( "rating: \"%1\"\n" ).arg( rating );
    res += QString( "ageRestrictions: \"%1\"\n" ).arg( ageRestrictions );
    res += QString( "tags: \"%1\"\n" ).arg( tags );
    res += QString( "viewsCounter: \"%1\"\n" ).arg( viewsCounter );
    res += QString( "isPosterExists: \"%1\"\n" ).arg( isPosterExists );
    res += QString( "isViewed: \"%1\"\n" ).arg( isViewed );
    res += QString( "isFavourite: \"%1\"\n" ).arg( isFavourite );
    res += QString( "screenwriter: \"%1\"\n" ).arg( screenwriter );
    res += QString( "composer: \"%1\"\n" ).arg( composer );
    res += "-= End =-\n";

    return( res );
}
#endif // QT_DEBUG

bool Film::SetYearFromStr( const QString& s )
{
    bool ok;
    year = s.toUInt( &ok );
    return( ok );
}

bool Film::SetRatingFromStr( const QString& s )
{
    bool ok;
    rating = qRound( s.toDouble( &ok ) );
    if( !ok ) rating = 1;
    return( ok );
}

bool Film::SetBudgetFromStr( const QString& s )
{
    bool ok;
    budget = s.toDouble( &ok );
    if( !ok ) budget = 0;
    return( ok );
}

void Film::SetNewData( const Film& other )
{
    DebugPrintFuncA( "Film::SetNewData", other.GetFileName() );

    section = other.section;
    fileName = other.fileName;
    title = other.title;
    originalTitle = other.originalTitle;
    tagline = other.tagline;
    genre = other.genre;
    country = other.country;
    year = other.year;
    director = other.director;
    producer = other.producer;
    starring = other.starring;
    budget = other.budget;
    description = other.description;
    rating = other.rating;
    ageRestrictions = other.ageRestrictions;
    tags = other.tags;
    viewsCounter = other.viewsCounter;
    isPosterExists = other.isPosterExists;
    isViewed = other.isViewed;
    isFavourite = other.isFavourite;
    screenwriter = other.screenwriter;
    composer = other.composer;
}

QString Film::GetRandomHash()
{
    DebugPrintFunc( "Film::GetRandomHash" );

    QByteArray hash = QCryptographicHash::hash( QByteArray::number( qrand() ), QCryptographicHash::Sha1 );
    return( QString( hash.toHex() ) );
}

QString Film::ClearTitle( QString title )
{
    QString newTitle = title;

    static QStringList wordsToRemove;

    if( wordsToRemove.isEmpty() )
    {
        // TODO: comment this
        wordsToRemove = QStringList{ "DHT-Movies", "TheaterRip", "BlueBird", "HDTV720", "HDTVRip", "NeroAVC",
                                     "UNRATED", "BluRay", "DVDRip", "HDclub", "HDRip", "WEB-DL", "1080p", "BDRip",
                                     "720p", "H264", "HDTV", "HiDt", "x264", "AC3", "AVC", "AVO", "DTS", "Eng",
                                     "JRG", "MKV", "MVO", "Fra", "Rus", "Ukr", "HD" };
    }

    for( const QString& word : wordsToRemove )
    {
        newTitle.replace( word, "", Qt::CaseInsensitive );
    }

    // Years between 1850 and 2019
    newTitle.replace( QRegExp( "(185[0-9]|18[6-9][0-9]|19[0-9]{2}|200[0-9]|201[0-9])" ), "" );

    // Round and square brackets with the contents
    newTitle.replace( QRegExp( "\\[(.+)\\]|\\((.+)\\)" ), "" );

    // Round and square brackets whithout contents
    newTitle.replace( QRegExp( "\\(\\)|\\[\\]" ), "" );

    // Dots, underscores and dashes (hyphens)
    newTitle.replace( QRegExp( "\\.|_|\\-" ), " " );

    // Phrases like "NxSomething"
    newTitle.replace( QRegExp( "[0-9][xX]" ), "" );

    // Multiple spaces
    newTitle.replace( QRegExp( " {1,}" ), " " );

    // Spaces at begin end end of the string
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
