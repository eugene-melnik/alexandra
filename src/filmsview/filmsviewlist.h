/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewlist.h                                                                         *
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

#ifndef FILMSVIEWLIST_H
#define FILMSVIEWLIST_H

#include <QColor>
#include <QKeyEvent>
#include <QPoint>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "abstractfilmsview.h"
#include "filmslist/film.h"

class FilmsViewList : public QTableWidget, public AbstractFilmsView
{
    Q_OBJECT

    public:
        explicit FilmsViewList( QWidget* parent = nullptr );

    public slots:
        void LoadSettings();
        void ReloadSettings();
        void SaveSettings() const;

        int AddItem( const Film& film, QColor background = QColor() );
        void SetItem( int n, const Film& film, QColor background = QColor() );
        void SetCurrentItemTo( const Film film );

        void RemoveItem( int n );
        void RemoveItemByTitle( const QString& title );
        void RemoveCurrentItem();
        void Clear();

        void SelectItem( const Film& film );
        void SelectItem( const QString& title );
        void SelectRandomItem();

        int GetItemsCount() const;
        int GetCurrentItemIndex() const;
        QStringList GetSelectedItemsList() const;

        void SetCurrentItemIndex( int i );

    signals:
        void ItemClicked( const QString& );
        void ItemDoubleClicked( const QString& );
        void ContextMenuRequested( QPoint );

    protected:
        void keyPressEvent( QKeyEvent* event );

    private slots:
        void ItemClickedSlot();
        void ItemDoubleClickedSlot();
        void ContextMenuRequestedSlot( QPoint p );

    private:
        enum Columns {
            ViewedColumn,
            FavouriteColumn,
            TitleColumn,
            YearColumn,
            GenreColumn,
            DirectorColumn,
            RatingColumn
        };

        const QStringList colNames = {
            tr( "V" ),  // Is viewed
            tr( "F" ),  // Is favourite
            tr( "Title" ),
            tr( "Year" ),
            tr( "Genre" ),
            tr( "Director" ),
            tr( "Rating" )
        };

        const QStringList colTooltips = {
            tr( "Is viewed" ),
            tr( "Is favourite" ),
            tr( "Title" ),
            tr( "Year" ),
            tr( "Genre" ),
            tr( "Director" ),
            tr( "Rating" )
        };
};

#endif // FILMSVIEWLIST_H
