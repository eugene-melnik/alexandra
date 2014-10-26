#ifndef FILMSLIST_H
#define FILMSLIST_H

#include <QTableWidget>
#include <QString>
#include <QList>

#include "film.h"

class FilmsList : public QTableWidget
{
    public:
        FilmsList( QWidget* parent = nullptr );

        void LoadDatabase( QString dataDirectory );
        void SaveDatabase( QString dataDirectory );

        void AppendFilm( Film f );

        int GetNumberOfFilms() const;
        const Film& GetFilmAt( int i) const;
        const Film& GetFilmByTitle( const QString& t ) const;

        int GetIsViewedCount() const;
        int GetIsFavouriteCount() const;

        void UpdateFilmsTable();

    private:
        QList<Film> films;
};

#endif // FILMSLIST_H
