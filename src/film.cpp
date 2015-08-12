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

/*************************************************************************************************
 *  Constructors                                                                                  *
  *************************************************************************************************/

Film::Film()
{
    viewsCounter = 0;
    isPosterExists = false;
    isViewed = false;
    isFavourite = false;
}

Film::Film( const Film& other )
{
    id = other.id;
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
    additionalText = other.additionalText;
    viewsCounter = other.viewsCounter;
    isPosterExists = other.isPosterExists;
    isViewed = other.isViewed;
    isFavourite = other.isFavourite;
}

/*************************************************************************************************
 *  Operators' overloads                                                                          *
  *************************************************************************************************/

bool Film::operator > ( const Film& other ) const
{
    return( title > other.title );
}

bool Film::operator < ( const Film& other ) const
{
    return( title < other.title );
}

bool Film::operator == ( const Film& other ) const
{
    return( title == other.title );
}

const QString &Film::GetId() const
{
    return( id );
}

QDataStream& operator << ( QDataStream& out, const Film& f )
{
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
        << f.additionalText
        << f.viewsCounter
        << f.isPosterExists
        << f.isViewed
        << f.isFavourite;

    return( out );
}

QDataStream& operator >> ( QDataStream& in, Film& f )
{
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
       >> f.additionalText
       >> f.viewsCounter
       >> f.isPosterExists
       >> f.isViewed
       >> f.isFavourite;

    return( in );
}

/*************************************************************************************************
 *  Getters                                                                                       *
  *************************************************************************************************/

const QString& Film::GetFileName() const
{
    return( fileName );
}

const QString& Film::GetTitle() const
{
    return( title );
}

const QString& Film::GetOriginalTitle() const
{
    return( originalTitle );
}

const QString& Film::GetTagline() const
{
    return( tagline );
}

const QString& Film::GetGenre() const
{
    return( genre );
}

const QString& Film::GetCountry() const
{
    return( country );
}

quint16 Film::GetYear() const
{
    return( year );
}

QString Film::GetYearStr() const
{
    return( QString::number( year ) );
}

const QString& Film::GetDirector() const
{
    return( director );
}

const QString& Film::GetProducer() const
{
    return( producer );
}

const QString& Film::GetStarring() const
{
    return( starring );
}

const QString& Film::GetDescription() const
{
    return( description );
}

quint8 Film::GetRating() const
{
    return( rating );
}

QString Film::GetRatingStr() const
{
    return( QString( "%1/10" ).arg( rating ) );
}

const QString &Film::GetTags() const
{
    return( tags );
}

int Film::GetViewsCounter() const
{
    return( viewsCounter );
}

bool Film::GetIsPosterExists() const
{
    return( isPosterExists );
}

QString Film::GetPosterName() const
{
    return( GetId() + ".png" );
}

bool Film::GetIsViewed() const
{
    return( isViewed );
}

QString Film::GetIsViewedSign() const
{
    return( isViewed ? "+" : "-" );
}

bool Film::GetIsFavourite() const
{
    return( isFavourite );
}

QString Film::GetIsFavouriteSign() const
{
    return( isFavourite ? "+" : "-" );
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
    res += QString( "director: \"%1\"\n" ).arg( director );
    res += QString( "producer: \"%1\"\n" ).arg( producer );
    res += QString( "starring: \"%1\"\n" ).arg( starring );
    res += QString( "budget: \"%1\"\n" ).arg( budget );
    res += QString( "description: \"%1\"\n" ).arg( description );
    res += QString( "rating: \"%1\"\n" ).arg( rating );
    res += QString( "ageRestrictions: \"%1\"\n" ).arg( ageRestrictions );
    res += QString( "tags: \"%1\"\n" ).arg( tags );
    res += QString( "additionalText: \"%1\"\n" ).arg( additionalText );
    res += QString( "viewsCounter: \"%1\"\n" ).arg( viewsCounter );
    res += QString( "isPosterExists: \"%1\"\n" ).arg( isPosterExists );
    res += QString( "isViewed: \"%1\"\n" ).arg( isViewed );
    res += QString( "isFavourite: \"%1\"\n" ).arg( isFavourite );
    res += "-= End =-\n";

    return( res );
}

#endif // QT_DEBUG

/*************************************************************************************************
 *  Setters                                                                                       *
  *************************************************************************************************/

void Film::SetId( const QString &s )
{
    id = s;
}

void Film::SetFileName( const QString& s )
{
    fileName = s;
}

void Film::SetTitle( const QString& s )
{
    title = s;
}

void Film::SetOriginalTitle( const QString& s )
{
    originalTitle = s;
}

void Film::SetTagline( const QString& s )
{
    tagline = s;
}

void Film::SetGenre( const QString& s )
{
    genre = s;
}

void Film::SetCountry( const QString& s )
{
    country = s;
}

void Film::SetYear( quint16 n )
{
    year = n;
}

bool Film::SetYearFromStr( const QString& s )
{
    bool ok;
    year = s.toUInt( &ok );
    return( ok );
}

void Film::SetDirector( const QString& s )
{
    director = s;
}

void Film::SetProducer( const QString& s )
{
    producer = s;
}

void Film::SetStarring( const QString& s )
{
    starring = s;
}

void Film::SetDescription( const QString& s )
{
    description = s;
}

void Film::SetRating( quint8 n )
{
    rating = n;
}

bool Film::SetRatingFromStr( const QString& s )
{
    bool ok;
    rating = s.toUInt( &ok );
    return( ok );
}

void Film::SetTags( const QString &s )
{
    tags = s;
}

void Film::IncViewsCounter()
{
    viewsCounter++;
}

void Film::SetViewCounter( int count )
{
    viewsCounter = count;
}

void Film::SetIsPosterExists( bool b )
{
    isPosterExists = b;
}

void Film::SetIsViewed( bool b )
{
    isViewed = b;
}

void Film::SetIsFavourite( bool b )
{
    isFavourite = b;
}

void Film::SetNewData( const Film& other )
{
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
    additionalText = other.additionalText;
    viewsCounter = other.viewsCounter;
    isPosterExists = other.isPosterExists;
    isViewed = other.isViewed;
    isFavourite = other.isFavourite;
}
