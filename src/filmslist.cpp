#include "filmslist.h"
#include "version.h"

#include <QDataStream>
#include <QFile>

void FilmsList::ReadDatabase( QString dataDirectory )
{
    dataDirectory.append( "database.dat" );
    QFile dbFile( dataDirectory );

    if( dbFile.open( QIODevice::ReadOnly ) )
    {
        QDataStream stream( &dbFile );
        stream >> films;
    }

    dbFile.close();
}

void FilmsList::WriteDatabase( QString dataDirectory )
{
    dataDirectory.append( "database.dat" );
    QFile dbFile( dataDirectory );

    if( dbFile.open( QIODevice::WriteOnly ) )
    {
        QDataStream stream( &dbFile );
        stream << films;
    }

    dbFile.close();
}

void FilmsList::append( Film f )
{
    films.append( f );
}

const Film& FilmsList::at( int i ) const
{
    return( films.at( i ) );
}

int FilmsList::size() const
{
    return( films.size() );
}

const Film* FilmsList::GetFilmByTitle( const QString& t ) const
{
    for( int i = 0; i < films.size(); i++ )
    {
        if( films.at(i).title == t )
        {
            return( &films.at(i) );
        }
    }

    return( nullptr );
}

int FilmsList::GetIsViewedCount() const
{
    int res = 0;

    foreach (Film f, films)
    {
        if( f.isViewed ) res++;
    }

    return( res );
}

int FilmsList::GetIsFavouriteCount() const
{
    int res = 0;

    foreach (Film f, films)
    {
        if( f.isFavourite ) res++;
    }

    return( res );
}
