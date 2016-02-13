/*************************************************************************************************
 *                                                                                                *
 *  file: foundedlistwidget.h                                                                     *
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

#ifndef FOUNDEDLISTWIDGET_H
#define FOUNDEDLISTWIDGET_H


#include "ui_foundedlistwidget.h"


#include <QWidget>


class FoundedListWidget : public QWidget, public Ui::FoundedListWidget
{
    Q_OBJECT

    public:
        explicit FoundedListWidget( QWidget* parent = nullptr );

        void AppendItem( QTableWidgetItem* item );
        void Clear();

        void ScrollToItem( QTableWidgetItem* item ) { twFounded->scrollToItem( item, QAbstractItemView::PositionAtCenter ); }

        int GetItemsCount() const { return( twFounded->rowCount() ); }
        QList<QTableWidgetItem*> GetItems() const;

    signals:
        void ItemsCountChanged( int newCount );
        void SelectionChanged( int selectedCount );

    private slots:
        void SelectAll();
        void UnselectAll();
        void InvertSelection();
        void CalculateSelected();
};


#endif // FOUNDEDLISTWIDGET_H
