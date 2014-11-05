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
        void EditCurrentFilm( Film f );

        int GetNumberOfFilms() const;
        const Film* GetFilmAt( int i ) const;
        const Film* GetFilmByTitle( const QString& t ) const;

        const Film* GetCurrentFilm() const;
        const QString& GetCurrentFilmTitle() const;
        const QString& GetCurrentFilmFileName() const;
        void RemoveCurrentFilm();

        int GetIsViewedCount() const;
        int GetIsFavouriteCount() const;

    public slots:
        void ItemSelected( QTableWidgetItem* i );
        void UpdateFilmsTable();

    signals:
        void FilmSelected( const Film* );
        void DatabaseChanged();

    private:
        QList<Film> films;
        const Film* currentFilm;
        bool isDatabaseChanged;
};

#endif // FILMSLIST_H
