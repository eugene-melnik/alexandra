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
#include <QKeyEvent>
#include <QListView>
#include <QStringList>

#include "abstractfilmsview.h"
#include "filmslist/film.h"
#include "filmsviewgridmodel.h"

class FilmsViewGrid : public QListView, public AbstractFilmsView
{
    Q_OBJECT

    public:
        explicit FilmsViewGrid( QWidget* parent = nullptr );

    public slots:
        void LoadSettings();
        void ReloadSettings();
        void SaveSettings() const {}

        int  AddItem( const Film& film );
        void SetItem( int n, const Film& film ) { model->SetItem( n, film ); }
        void SetCurrentItemTo( const Film film ) { SetItem( GetCurrentItemIndex(), film ); }

        void RemoveItem( int n );
        void RemoveItemByTitle( const QString& title );
        void RemoveCurrentItem() { RemoveItem( GetCurrentItemIndex() ); }
        void Clear() { model->Clear(); }

        void SelectItem( const Film& film ) { SelectItem( film.GetTitle() ); }
        void SelectItem( const QString& title ) { SetCurrentItemIndex( model->GetItemIndexByTitle( title ) ); }
        void SelectRandomItem();

        int GetItemsCount() const { return( model->rowCount() ); }
        int GetCurrentItemIndex() const { return( currentIndex().row() ); }
        QStringList GetSelectedItemsList() const;

        void SetCurrentItemIndex( int i );

        void FilterBy( const QStringList& titles );
        void ResetFilter();

    signals:
        void ItemClicked( const QString& );
        void ItemDoubleClicked( const QString& );
        void ContextMenuRequested( QPoint );

    protected:
        void keyPressEvent( QKeyEvent* event );
        void updateGeometries();

    private slots:
        void ItemClickedSlot( QModelIndex i );
        void ItemDoubleClickedSlot( QModelIndex i );
        void ContextMenuRequestedSlot( QPoint p );

    private:
        FilmViewGridModel* model = nullptr;
};

#endif // FILMSVIEWGRID_H
