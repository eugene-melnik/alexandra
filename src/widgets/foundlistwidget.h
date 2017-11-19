/*************************************************************************************************
 *                                                                                                *
 *  file: foundlistwidget.h                                                                       *
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

#ifndef FOUNDLISTWIDGET_H
#define FOUNDLISTWIDGET_H


#include "ui_foundlistwidget.h"


#include <QWidget>


class FoundListWidget : public QWidget, public Ui::FoundListWidget
{
    Q_OBJECT

    public:
        explicit FoundListWidget( QWidget* parent = nullptr );

        void AddItem( QString itemText, bool setDisabled = false );
        void AddItem( QString itemText, QVariant data, bool setDisabled = false );
        void DisableItem( QString itemText, bool scrollToItem = false );
        void Clear();

        QStringList GetItems() const;
        QStringList GetSelectedItems() const { return( this->lwFound->GetSelectedItems() ); }
        QList<QPair<QString,QVariant>> GetSelectedItemsData() const { return( this->lwFound->GetSelectedItemsData() ); }
        int GetItemsCount() const { return( this->lwFound->count() ); }

    signals:
        void ItemsCountChanged( int newCount );
        void SelectionChanged( int selectedCount );

    private slots:
        void CalculateSelected();

    private:
        QColor disabledColor = QColor( 0, 255, 0, 40 ); // light green
};


#endif // FOUNDLISTWIDGET_H
