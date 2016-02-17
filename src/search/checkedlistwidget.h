/*************************************************************************************************
 *                                                                                                *
 *  file: checkedlistwidget.h                                                                     *
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

#ifndef CHECKEDLISTWIDGET_H
#define CHECKEDLISTWIDGET_H


#include <QListWidget>
#include <QMenu>
#include <QPoint>
#include <QStringList>


class CheckedListWidget : public QListWidget
{
    Q_OBJECT

    public:
        explicit CheckedListWidget( QWidget* parent = nullptr ) : QListWidget( parent )
        {
            setContextMenuPolicy( Qt::CustomContextMenu );
            connect( this, &QWidget::customContextMenuRequested, this, &CheckedListWidget::ShowContextMenu );
        }

        void AddItem( QString itemTitle )
        {
            QListWidgetItem* item = new QListWidgetItem( itemTitle, this );
            item->setCheckState( Qt::Unchecked );
            addItem( item );
        }

        void AddItems( QStringList itemsTitles )
        {
            for( QString title : itemsTitles )
            {
                AddItem( title );
            }
        }

        QStringList GetSelectedItems() const
        {
            QStringList result;

            for( int row = 0; row < count(); ++row )
            {
                if( item(row)->checkState() == Qt::Checked )
                {
                    result.append( item(row)->text() );
                }
            }

            return( result );
        }

    private slots:
        void ShowContextMenu( QPoint pos )
        {
            if( count() > 0 )
            {
                QMenu menu;
                menu.addAction( "Select all", this, SLOT(SelectAll()) );
                menu.addAction( "Unselect all", this, SLOT(UnselectAll()) );
                menu.addAction( "Invert selection", this, SLOT(InvertSelection()) );
                menu.exec( this->mapToGlobal(pos) );
            }
        }

        void SelectAll()
        {
            for( int row = 0; row < count(); ++row )
            {
                item(row)->setCheckState( Qt::Checked );
            }
        }

        void UnselectAll()
        {
            for( int row = 0; row < count(); ++row )
            {
                item(row)->setCheckState( Qt::Unchecked );
            }
        }

        void InvertSelection()
        {
            for( int row = 0; row < count(); ++row )
            {
                item(row)->setCheckState( (item(row)->checkState() == Qt::Unchecked) ? Qt::Checked : Qt::Unchecked );
            }
        }
};


#endif // CHECKEDLISTWIDGET_H

