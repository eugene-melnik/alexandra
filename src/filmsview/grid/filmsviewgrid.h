/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewgrid.h                                                                         *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2017 Eugene Melnik <eugene@melnik.solutions>                               *
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


#include <QListView>


#include "../abstractfilmsview.h"
#include "filmsviewgridproxymodel.h"


class FilmsViewGrid : public QListView, public AbstractFilmsView
{
    Q_OBJECT

    public:
        explicit FilmsViewGrid( QWidget* parent = nullptr );

        void setModel( QAbstractItemModel* model ) override;

    public slots:
        void LoadSettings() override { ReloadSettings(); }
        void ReloadSettings() override;

        int GetRowCount() override { return( proxyModel->rowCount() ); }
        QModelIndex GetCurrentIndex() override { return( proxyModel->mapToSource( currentIndex() ) ); }
        QModelIndexList GetSelectedItemsList() override;

        void SetCurrentIndex( const QModelIndex& index ) override { setCurrentIndex( proxyModel->mapFromSource(index) ); }
        void SetCurrentRow( int row ) override { setCurrentIndex( model()->index( row, 0 ) ); }

        void ScrollToCurrent() override { scrollTo( currentIndex() ); }

    signals:
        void CurrentChanged( const QModelIndex& );
        void CurrentActivated( const QModelIndex& );
        void ContextMenuRequested( const QPoint&, const QModelIndex& );

    protected:
        void updateGeometries() override;

    private slots:
        void MapCurrentChanged( const QModelIndex& index );

    private:
        FilmsViewGridProxyModel* proxyModel;
};


#endif // FILMSVIEWGRID_H

