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

const Film& FilmsList::at( int i ) const
{
    return( films.at( i ) );
}

int FilmsList::size() const
{
    return( films.size() );
}
