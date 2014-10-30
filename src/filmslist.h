#ifndef FILMSLIST_H
#define FILMSLIST_H

#include <QTableWidget>
#include <QSettings>
#include <QString>
#include <QList>

#include "film.h"

class FilmsList : public QTableWidget
{
    Q_OBJECT

    public:
        FilmsList( QWidget* parent = nullptr );

        void LoadDatabase( QString dataDirectory );
        void SaveDatabase( QString dataDirectory );

        void LoadSettings( const QSettings& s );
        void SaveSettings( QSettings& s ) const;

        void AppendFilm( Film f );

        int GetNumberOfFilms() const;
        const Film& GetFilmAt( int i) const;
        const Film& GetFilmByTitle( const QString& t ) const;

        int GetIsViewedCount() const;
        int GetIsFavouriteCount() const;

    public slots:
        void UpdateFilmsTable();

    signals:
        void DatabaseChanged();

    private:
        QList<Film> films;
};

#endif // FILMSLIST_H
