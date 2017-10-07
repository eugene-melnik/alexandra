/*************************************************************************************************
 *                                                                                                *
 *  file: foundlistwidget.cpp                                                                     *
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

#include "foundlistwidget.h"


FoundListWidget::FoundListWidget(QWidget* parent) : QWidget(parent)
{
    setupUi( this );
    connect( bScrollToChecked, &QPushButton::clicked, lwFound, &CheckedListWidget::ScrollToChecked );
    connect( bSelectAll,       &QPushButton::clicked, lwFound, &CheckedListWidget::SelectAll );
    connect( bSelectAll,       &QPushButton::clicked, this,    &FoundListWidget::CalculateSelected );
    connect( bUnselectAll,     &QPushButton::clicked, lwFound, &CheckedListWidget::UnselectAll );
    connect( bUnselectAll,     &QPushButton::clicked, this,    &FoundListWidget::CalculateSelected );
    connect( bInvertSelection, &QPushButton::clicked, lwFound, &CheckedListWidget::InvertSelection );
    connect( bInvertSelection, &QPushButton::clicked, this,    &FoundListWidget::CalculateSelected );
    connect( lwFound,          &QListWidget::clicked, this,    &FoundListWidget::CalculateSelected );
}


void FoundListWidget::AddItem( QString itemText, bool setDisabled )
{
    this->lwFound->AddItem( itemText, setDisabled, disabledColor );
    emit ItemsCountChanged( this->lwFound->count() );
}


void FoundListWidget::AddItem( QString itemText, QVariant data, bool setDisabled )
{
    this->lwFound->AddItem( itemText, data, setDisabled, disabledColor );
    emit ItemsCountChanged( this->lwFound->count() );
}


void FoundListWidget::DisableItem( QString itemText, bool scrollToItem )
{
    for( int row = 0; row < this->lwFound->count(); ++row )
    {
        QListWidgetItem* item = this->lwFound->item(row);

        if( item->text() == itemText )
        {
            item->setFlags( Qt::NoItemFlags );
            item->setCheckState( Qt::Unchecked );
            item->setBackgroundColor( disabledColor );

            if( scrollToItem )
            {
                this->lwFound->scrollToItem( item, QAbstractItemView::PositionAtCenter );
            }

            break;
        }
    }
}


void FoundListWidget::Clear()
{
    this->lwFound->clear();
    emit ItemsCountChanged( 0 );
    emit SelectionChanged( 0 );
}


QStringList FoundListWidget::GetItems() const
{
    QStringList items;

    for( int row = 0; row < this->lwFound->count(); ++row )
    {
        items.append( this->lwFound->item(row)->text() );
    }

    return( items );
}


void FoundListWidget::CalculateSelected()
{
    int count = 0;

    for( int row = 0; row < this->lwFound->count(); ++row )
    {
        QListWidgetItem* item = this->lwFound->item(row);

        if( item->checkState() == Qt::Checked )
        {
            count++;
        }
    }

    emit SelectionChanged( count );
}

