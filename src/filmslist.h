/*************************************************************************************************
 *                                                                                                *
 *  file: filmslist.h                                                                             *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>                                     *
 *                                                                                                *
 *  Alexandra is free software; you can redistribute it and/or modify it under the terms of the   *
 *  GNU General Public License as published by the Free Software Foundation; either version 2 of  *
 *  the License, or (at your option) any later version.                                           *
 *                                                                                                *
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     *
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     *
 *  See the GNU General Public License for more details.                                          *
 *                                                                                                *
 *  You should have received a copy of the GNU General Public License along with this program.    *
 *  If not, see <http://www.gnu.org/licenses/>.                                                   *
 *                                                                                                *
  *************************************************************************************************/

#ifndef FILMSLIST_H
#define FILMSLIST_H

#include <QList>
#include <QString>
#include <QTableWidget>

#include "alexandrasettings.h"
#include "film.h"

class FilmsList : public QTableWidget
{
    Q_OBJECT

    public:
        FilmsList( QWidget* parent = nullptr );

        void LoadDatabase();
        void SaveDatabase();

        void LoadSettings( AlexandraSettings* s );
        void SaveSettings() const;

        int GetNumberOfFilms() const;
        const Film* GetFilmAt( int i ) const;
        const QList<Film>* GetFilmsList() const;

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

        void SelectFilm( const QString& title );
        void ItemSelected( QTableWidgetItem* i );
        void UpdateFilmsTable();

        void FilterBy( QString s );

        void EraseDatabase();

    signals:
        void DatabaseReadError();
        void DatabaseIsWrong();
        void DatabaseIsEmpty();
        void DatabaseIsReadonly();

        void FilmSelected( const Film* );
        void DatabaseChanged();

    private slots:
        void SetCursorOnCurrentRow();

    private:
        void SetCursorOnRow( int row );
        void SetCursorOnFilm( const QString& title );
        void SaveDatabaseConcurrent( QString databaseFileName );

        // Variables
        QList<Film> films;
        Film* currentFilm = nullptr;
        AlexandraSettings* settings = nullptr;
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
