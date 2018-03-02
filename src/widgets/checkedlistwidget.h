/*************************************************************************************************
 *                                                                                                *
 *  file: checkedlistwidget.h                                                                     *
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

#ifndef CHECKEDLISTWIDGET_H
#define CHECKEDLISTWIDGET_H


#include <QList>
#include <QListWidget>
#include <QMenu>
#include <QPair>
#include <QPoint>
#include <QStringList>
#include <QTimer>


class CheckedListWidget : public QListWidget
{
    Q_OBJECT

    public:
        explicit CheckedListWidget( QWidget* parent = nullptr );

        void AddItem( QString itemText, bool setDisabled = false, QColor disabledColor = QColor() );
        void AddItem( QString itemText, QVariant data, bool setDisabled = false, QColor disabledColor = QColor() );
        void AddItems( QStringList itemsText );
        void Clear() { clear(); }

        QStringList GetSelectedItems() const;
        QList<QPair<QString,QVariant>> GetSelectedItemsData() const;

    public slots:
        void SelectAll() { SetAllChecked(Qt::Checked); }
        void UnselectAll() { SetAllChecked(Qt::Unchecked); }
        void InvertSelection();
        void ScrollToChecked();

    protected:
        void keyPressEvent( QKeyEvent* event ) override;

    private slots:
        void ShowContextMenu( QPoint pos );
        void SetAllChecked( Qt::CheckState checked );

        void ClearSearch() { searchText.clear(); searchTimer.stop(); }

    private:
        QString searchText;
        QTimer searchTimer;
};


#endif // CHECKEDLISTWIDGET_H

