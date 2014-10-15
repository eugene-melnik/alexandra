#include "film.h"

Film::Film()
{
    //
}

QDataStream& operator << ( QDataStream& out, const Film& f )
{
    out << f.fileName << f.title << f.originalTitle << f.tagline << f.genre
        << f.country << f.year << f.director << f.producer << f.starring
        << f.description << f.rating << f.poster;

    return( out );
}

QDataStream& operator >> ( QDataStream& in, Film& f )
{
    in >> f.fileName >> f.title >> f.originalTitle >> f.tagline >> f.genre
       >> f.country >> f.year >> f.director >> f.producer >> f.starring
       >> f.description >> f.rating >> f.poster;

    return( in );
}
