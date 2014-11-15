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
        void SetCursorOnRow( int row );

        // Variables
        QList<Film> films;
        Film* currentFilm;
        bool isDatabaseChanged;

        // Structures and enumerations
        enum ColumnIndexes {
            ViewedColumn    = 0,
            FavouriteColumn = 1,
            TitleColumn     = 2,
            YearColumn      = 3,
            GenreColumn     = 4,
            DirectorColumn  = 5,
            RatingColumn    = 6
        };
};

#endif // FILMSLIST_H
