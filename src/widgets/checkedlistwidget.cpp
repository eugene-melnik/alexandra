/*************************************************************************************************
 *                                                                                                *
 *  file: checkedlistwidget.cpp                                                                   *
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

#include "checkedlistwidget.h"
#include "tools/debug.h"

#include <QKeyEvent>


CheckedListWidget::CheckedListWidget( QWidget* parent ) : QListWidget( parent )
{
    setEditTriggers( QAbstractItemView::NoEditTriggers );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setSelectionMode( QAbstractItemView::SingleSelection );
    setContextMenuPolicy( Qt::CustomContextMenu );

    connect( this, &QWidget::customContextMenuRequested, this, &CheckedListWidget::ShowContextMenu );
    connect( &searchTimer, &QTimer::timeout, this, &CheckedListWidget::ClearSearch );
}


void CheckedListWidget::AddItem( QString itemText , bool setDisabled, QColor disabledColor )
{
    AddItem( itemText, QVariant(), setDisabled, disabledColor );
}


void CheckedListWidget::AddItem( QString itemText, QVariant data, bool setDisabled, QColor disabledColor )
{
    QListWidgetItem* item = new QListWidgetItem( itemText, this );
    item->setToolTip( itemText );
    item->setCheckState( Qt::Unchecked );
    item->setData( Qt::UserRole, data );

    if( setDisabled )
    {
        item->setFlags( Qt::NoItemFlags );
        item->setBackgroundColor( disabledColor );
    }

    addItem( item );
}


void CheckedListWidget::AddItems( QStringList itemsText )
{
    for( QString text : itemsText )
    {
        AddItem( text );
    }
}


QStringList CheckedListWidget::GetSelectedItems() const
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


QList<QPair<QString,QVariant>> CheckedListWidget::GetSelectedItemsData() const
{
    QList<QPair<QString,QVariant>> itemsData;

    for( int row = 0; row < count(); ++row )
    {
        QListWidgetItem* it = item(row);

        if( it->checkState() == Qt::Checked )
        {
            QPair<QString,QVariant> pair;
            pair.first = it->text();
            pair.second = it->data( Qt::UserRole );
            itemsData.append( pair );
        }
    }

    return( itemsData );
}


void CheckedListWidget::InvertSelection()
{
    for( int row = 0; row < count(); ++row )
    {
        if( item(row)->flags() != Qt::NoItemFlags )
        {
            item(row)->setCheckState( (item(row)->checkState() == Qt::Unchecked) ? Qt::Checked : Qt::Unchecked );
        }
    }
}


void CheckedListWidget::ScrollToChecked()
{
    for( int row = 0; row < count(); ++row )
    {
        if( item(row)->checkState() == Qt::Checked )
        {
            setCurrentItem( item(row) );
            break;
        }
    }
}


void CheckedListWidget::keyPressEvent( QKeyEvent* event )
{
    if( event->text().isEmpty() )
    {
        QListWidget::keyPressEvent(event);
    }
    else
    {
        searchText.append( event->text() );
        searchTimer.stop();

          // Search for an item by the text
        for( int row = 0; row < count(); ++row )
        {
            QListWidgetItem* it = item( row );
            QString itemText = it->text().left( searchText.length() );

            if( itemText.compare( searchText, Qt::CaseInsensitive ) == 0 )
            {
                setCurrentItem( it );
                break;
            }
        }

        searchTimer.start( 1000 ); // 1 second
    }
}


void CheckedListWidget::ShowContextMenu( QPoint pos )
{
    if( count() > 0 )
    {
        QMenu menu;
        menu.addAction( tr("Select all"), this, SLOT(SelectAll()) );
        menu.addAction( tr("Unselect all"), this, SLOT(UnselectAll()) );
        menu.addAction( tr("Invert selection"), this, SLOT(InvertSelection()) );
        menu.addSeparator();
        menu.addAction( tr("Scroll to checked"), this, SLOT(ScrollToChecked()) );
        menu.exec( this->mapToGlobal(pos) );
    }
}


void CheckedListWidget::SetAllChecked( Qt::CheckState checked )
{
    for( int row = 0; row < count(); ++row )
    {
        if( item(row)->flags() != Qt::NoItemFlags )
        {
            item(row)->setCheckState( checked );
        }
    }
}

