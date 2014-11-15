#include "film.h"

/*
 *  Constructors
 */

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

/*
 *  Operators' overloads
 */

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
        << f.poster
        << f.isViewed
        << f.isFavourite;

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
       >> f.poster
       >> f.isViewed
       >> f.isFavourite;

    return( in );
}

/*
 *  Getters
 */

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

const QPixmap& Film::GetPoster() const
{
    return( poster );
}

bool Film::GetIsViewed() const
{
    return( isViewed );
}

bool Film::GetIsFavourite() const
{
    return( isFavourite );
}

QString Film::GetIsFavouriteSign() const
{
    if( isFavourite ) {
        return( "+" );
    } else {
        return( "-" );
    }
}

/*
 *  Setters
 */

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

void Film::SetPoster( const QPixmap& p )
{
    poster = p;
}

bool Film::SetPosterFromFile( const QString& s )
{
    return( poster.load( s ) );
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
