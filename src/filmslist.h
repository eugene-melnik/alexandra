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

    private:
        QList<Film> films;
};

#endif // FILMSLIST_H
