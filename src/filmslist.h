#ifndef FILMSLIST_H
#define FILMSLIST_H

#include <QString>
#include <QList>

#include "film.h"

class FilmsList
{
    public:
        //FilmsList();

        void ReadDatabase( QString dataDirectory );
        void WriteDatabase( QString dataDirectory );

        void append( Film f );
        const Film& at( int i) const;
        int size() const;

        const Film* GetFilmByTitle( const QString& t ) const;

        int GetIsViewedCount() const;
        int GetIsFavouriteCount() const;

    private:
        QList<Film> films;
};

#endif // FILMSLIST_H
