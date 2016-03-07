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


FoundedListWidget::FoundedListWidget(QWidget* parent) : QWidget(parent)
{
    setupUi( this );
    connect( bScrollToChecked, &QPushButton::clicked, lwFounded, &CheckedListWidget::ScrollToChecked );
    connect( bSelectAll,       &QPushButton::clicked, lwFounded, &CheckedListWidget::SelectAll );
    connect( bSelectAll,       &QPushButton::clicked, this,      &FoundedListWidget::CalculateSelected );
    connect( bUnselectAll,     &QPushButton::clicked, lwFounded, &CheckedListWidget::UnselectAll );
    connect( bUnselectAll,     &QPushButton::clicked, this,      &FoundedListWidget::CalculateSelected );
    connect( bInvertSelection, &QPushButton::clicked, lwFounded, &CheckedListWidget::InvertSelection );
    connect( bInvertSelection, &QPushButton::clicked, this,      &FoundedListWidget::CalculateSelected );
    connect( lwFounded,        &QListWidget::clicked, this,      &FoundedListWidget::CalculateSelected );
}


void FoundedListWidget::AddItem( QString itemText, bool setDisabled )
{
    lwFounded->AddItem( itemText, setDisabled, disabledColor );
    emit ItemsCountChanged( lwFounded->count() );
}


void FoundedListWidget::AddItem( QString itemText, QVariant data, bool setDisabled )
{
    lwFounded->AddItem( itemText, data, setDisabled, disabledColor );
    emit ItemsCountChanged( lwFounded->count() );
}


void FoundedListWidget::DisableItem( QString itemText, bool scrollToItem )
{
    for( int row = 0; row < lwFounded->count(); ++row )
    {
        QListWidgetItem* item = lwFounded->item(row);

        if( item->text() == itemText )
        {
            item->setFlags( Qt::NoItemFlags );
            item->setCheckState( Qt::Unchecked );
            item->setBackgroundColor( disabledColor );

            if( scrollToItem )
            {
                lwFounded->scrollToItem( item, QAbstractItemView::PositionAtCenter );
            }

            break;
        }
    }
}


void FoundedListWidget::Clear()
{
    lwFounded->clear();
    emit ItemsCountChanged( 0 );
    emit SelectionChanged( 0 );
}


QStringList FoundedListWidget::GetItems() const
{
    QStringList items;

    for( int row = 0; row < lwFounded->count(); ++row )
    {
        items.append( lwFounded->item(row)->text() );
    }

    return( items );
}


void FoundedListWidget::CalculateSelected()
{
    int count = 0;

    for( int row = 0; row < lwFounded->count(); ++row )
    {
        QListWidgetItem* item = lwFounded->item(row);

        if( item->checkState() == Qt::Checked )
        {
            count++;
        }
    }

    emit SelectionChanged( count );
}

