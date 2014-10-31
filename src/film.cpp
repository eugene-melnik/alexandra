#include "film.h"

Film::Film()
{
    isViewed = false;
    isFavourite = false;
}

Film::Film( const Film& other )
{
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
    description = other.description;
    rating = other.rating;
    poster = other.poster;
    isViewed = other.isViewed;
    isFavourite = other.isFavourite;
}

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

QDataStream& operator << ( QDataStream& out, const Film& f )
{
    out << f.fileName
        << f.title
        << f.originalTitle
        << f.tagline
        << f.genre
        << f.country
        << f.year
        << f.director
        << f.producer
        << f.starring
        << f.description
        << f.rating
        << f.poster;

    return( out );
}

QDataStream& operator >> ( QDataStream& in, Film& f )
{
    in >> f.fileName
       >> f.title
       >> f.originalTitle
       >> f.tagline
       >> f.genre
       >> f.country
       >> f.year
       >> f.director
       >> f.producer
       >> f.starring
       >> f.description
       >> f.rating
       >> f.poster;

    return( in );
}
