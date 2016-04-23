/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewdetailedlist.h                                                                 *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2016 Eugene Melnik <jeka7js@gmail.com>                                     *
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

#ifndef FILMSVIEWDETAILEDLIST_H
#define FILMSVIEWDETAILEDLIST_H


#include <QTableWidget>


#include "../abstractfilmsview.h"


class FilmsViewDetailedList : public QTableWidget, public AbstractFilmsView
{
    Q_OBJECT

    public:
        explicit FilmsViewDetailedList( QWidget* parent = nullptr );

        void setModel( QAbstractItemModel* model ) override;

    public slots:
        int GetRowCount() override { return( model()->rowCount() ); }
        QModelIndex GetCurrentIndex() override { return( currentIndex() ); }
        QModelIndexList GetSelectedItemsList() override { return( selectedIndexes() ); }

        void SetCurrentIndex( const QModelIndex& index ) override { setCurrentIndex(index); }
        void SetCurrentRow( int row ) override { SetCurrentIndex( model()->index(row, 0) ); }

        void ScrollToCurrent() override { scrollTo( GetCurrentIndex() ); }

    signals:
        void CurrentChanged( const QModelIndex& );
        void CurrentActivated( const QModelIndex& );
        void ContextMenuRequested( const QPoint&, const QModelIndex& );

    protected slots:
        void ResetContents();

    private:
        QAbstractItemModel* sourceModel = nullptr;
};


#endif // FILMSVIEWDETAILEDLIST_H

