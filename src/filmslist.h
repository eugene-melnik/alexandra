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

        void LoadDatabase( QString databaseFileName );
        void SaveDatabase( QString databaseFileName );

        void LoadSettings( QSettings* s );
        void SaveSettings( QSettings* s ) const;

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
        void AppendFilm( Film f );
        void EditCurrentFilm( Film f );
        void SelectRandomFilm();

        void SetCurrentIsViewed( bool b );
        void SetCurrentIsFavourite( bool b );

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
