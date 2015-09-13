/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewgrid.h                                                                         *
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

#ifndef FILMSVIEWGRID_H
#define FILMSVIEWGRID_H

#include <QColor>
#include <QListView>

#include "abstractfilmsview.h"
#include "alexandrasettings.h"
#include "film.h"
#include "filmsviewgridmodel.h"

class FilmsViewGrid : public QListView, public AbstractFilmsView
{
    Q_OBJECT

    public:
        explicit FilmsViewGrid( QWidget* parent = nullptr );

    public slots:
        void LoadSettings( AlexandraSettings* s );
        void ReloadSettings( AlexandraSettings* s );
        void SaveSettings( AlexandraSettings* /*s*/ ) const {}

        int AddItem( const Film& film, QColor background = QColor() );
        void SetItem( int n, const Film& film, QColor background = QColor() );
        void SetCurrentItemTo( const Film film );

        void RemoveItem( int n );
        void RemoveCurrentItem();
        void Clear();

        void SelectItem( const Film& film );
        void SelectItem( const QString& title );
        void SelectRandomItem();

        int GetItemsCount() const;
        int GetCurrentItemIndex() const;

        void SetCurrentItemIndex( int i );

    signals:
        void ItemClicked( const QString& );
        void ItemDoubleClicked( const QString& );
        void ContextMenuRequested( QPoint );

    protected:
        void updateGeometries();

    private slots:
        void ItemClickedSlot( QModelIndex i );
        void ItemDoubleClickedSlot( QModelIndex i );
        void ContextMenuRequestedSlot( QPoint p );

    private:
        AlexandraSettings* settings = nullptr;
        FilmViewGridModel* model = nullptr;
};

#endif // FILMSVIEWGRID_H
