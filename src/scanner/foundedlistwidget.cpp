/*************************************************************************************************
 *                                                                                                *
 *  file: foundedlistwidget.cpp                                                                   *
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

#include "foundedlistwidget.h"

#include <QTableWidgetItem>


FoundedListWidget::FoundedListWidget(QWidget* parent) : QWidget(parent)
{
    setupUi( this );

    connect( bSelectAll,       &QPushButton::clicked,  this, &FoundedListWidget::SelectAll );
    connect( bUnselectAll,     &QPushButton::clicked,  this, &FoundedListWidget::UnselectAll );
    connect( bInvertSelection, &QPushButton::clicked,  this, &FoundedListWidget::InvertSelection );
    connect( twFounded,        &QTableWidget::clicked, this, &FoundedListWidget::CalculateSelected );
}


void FoundedListWidget::AppendItem( QTableWidgetItem* item )
{
    item->setCheckState( Qt::Unchecked );
    int row = twFounded->rowCount();
    twFounded->setRowCount( row + 1 );
    twFounded->setItem( row, 0, item );
    emit ItemsCountChanged( row + 1 );
}


void FoundedListWidget::Clear()
{
    twFounded->clearContents();
    twFounded->setRowCount( 0 );
    emit ItemsCountChanged( 0 );
    emit SelectionChanged( 0 );
}


QList<QTableWidgetItem*> FoundedListWidget::GetItems() const
{
    QList<QTableWidgetItem*> items;

    for( int row = 0; row < twFounded->rowCount(); ++row )
    {
        items.append( twFounded->item( row, 0 ) );
    }

    return( items );
}


void FoundedListWidget::SelectAll()
{
    for( int i = 0; i < twFounded->rowCount(); ++i )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

        if( item->flags() != Qt::NoItemFlags )
        {
            item->setCheckState( Qt::Checked );
        }
    }

    CalculateSelected();
}


void FoundedListWidget::UnselectAll()
{
    for( int i = 0; i < twFounded->rowCount(); ++i )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

        if( item->flags() != Qt::NoItemFlags )
        {
            item->setCheckState( Qt::Unchecked );
        }
    }

    CalculateSelected();
}


void FoundedListWidget::InvertSelection()
{
    for( int i = 0; i < twFounded->rowCount(); ++i )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

        if( item->flags() != Qt::NoItemFlags )
        {
            if( item->checkState() == Qt::Unchecked )
            {
                item->setCheckState( Qt::Checked );
            }
            else
            {
                item->setCheckState( Qt::Unchecked );
            }
        }
    }

    CalculateSelected();
}


void FoundedListWidget::CalculateSelected()
{
    int count = 0;

    for( int i = 0; i < twFounded->rowCount(); ++i )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

        if( item->checkState() == Qt::Checked )
        {
            count++;
        }
    }

    emit SelectionChanged( count );
}

