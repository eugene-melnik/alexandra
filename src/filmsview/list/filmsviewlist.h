/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewlist.h                                                                         *
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

#ifndef FILMSVIEWLIST_H
#define FILMSVIEWLIST_H


#include <QKeyEvent>
#include <QTableView>


#include "../abstractfilmsview.h"


class FilmsViewList : public QTableView, public AbstractFilmsView
{
    Q_OBJECT

    public:
        explicit FilmsViewList( QWidget* parent = nullptr );

        void setModel( QAbstractItemModel* model ) override;

    public slots:
        void LoadSettings() override;
        void ReloadSettings() override;
        void SaveSettings() const override;

        int GetRowCount() override { return( model()->rowCount() ); }
        QModelIndex GetCurrentIndex() override { return( currentIndex() ); }
        QModelIndexList GetSelectedItemsList() override;

        void SetCurrentIndex( const QModelIndex& index ) override { setCurrentIndex(index); scrollTo(index, ScrollHint::PositionAtCenter); }
        void SetCurrentRow( int row ) override { SetCurrentIndex( model()->index( row, 0 ) ); }

        void ScrollToCurrent() override { currentChanged( currentIndex(), currentIndex() ); }

    signals:
        void CurrentChanged( const QModelIndex& );
        void CurrentActivated( const QModelIndex& );
        void ContextMenuRequested( const QPoint&, const QModelIndex& );

    protected:
        void keyPressEvent( QKeyEvent* event ) override;

    private slots:
        void ShowHeaderContextMenu( const QPoint& pos );
        void SetDefaultColumnsView();
        void RestoreColumnsOrder();
};


#endif // FILMSVIEWLIST_H

