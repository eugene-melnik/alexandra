/*************************************************************************************************
 *                                                                                                *
 *  file: playlistwidget.cpp                                                                      *
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

#include "playlistwidget.h"
#include "alexandrasettings.h"
#include "tools/debug.h"

#include <QAction>
#include <QMenu>


PlaylistWidget::PlaylistWidget( QWidget* parent ) : QWidget( parent )
{
    setupUi( this );
    hide();

    connect( lwMain, &QListWidget::customContextMenuRequested, this, &PlaylistWidget::ShowContextMenu );
    connect( lwMain, &QListWidget::doubleClicked, this, &PlaylistWidget::SelectClickedItem );
    connect( this,   &PlaylistWidget::Cleared,    this, &QWidget::hide );
}


void PlaylistWidget::LoadSettings()
{
    QByteArray data = AlexandraSettings::GetInstance()->GetPlaylistData();
    QDataStream stream( &data, QIODevice::ReadOnly );
    QString title, filePath;

    while( !stream.atEnd() )
    {
        stream >> title >> filePath;
        AddItem( title, filePath );
    }
}


void PlaylistWidget::SaveSettings() const
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );

    for( int row = 0; row < lwMain->count(); ++row )
    {
        QListWidgetItem* it = lwMain->item( row );

        stream << it->data(Qt::DisplayRole).toString()
               << it->data(Qt::UserRole).toString();
    }

    AlexandraSettings::GetInstance()->SetPlaylistData( data );
}


void PlaylistWidget::AddItem( const QString& title , const QString& filePath )
{
    DebugPrintFunc( "PlayListWidget::AddItem", title );

    QListWidgetItem* item = new QListWidgetItem( lwMain );
    item->setData( Qt::DisplayRole, title );
    item->setData( Qt::ToolTipRole, title );
    item->setData( Qt::UserRole, filePath );

    lwMain->addItem( item );
    lwMain->setCurrentItem( item );

    if( isHidden() )
    {
        show();
    }

    DebugPrintFuncDone( "PlayListWidget::AddItem" );
}


void PlaylistWidget::RemoveItem( const QString& title )
{
    for( int row = 0; row < lwMain->count(); ++row )
    {
        QListWidgetItem* it = lwMain->item(row);

        if( it->text() == title )
        {
            lwMain->setCurrentItem( it );
            RemoveCurrentFromList();
        }
    }
}


QStringList PlaylistWidget::GetTitles() const
{
    QStringList titles;

    for( int row = 0; row < lwMain->count(); ++row )
    {
        titles.append( lwMain->item(row)->data(Qt::DisplayRole).toString() );
    }

    return( titles );
}


QStringList PlaylistWidget::GetPathes() const
{
    QStringList pathes;

    for( int row = 0; row < lwMain->count(); ++row )
    {
        pathes.append( lwMain->item(row)->data(Qt::UserRole).toString() );
    }

    return( pathes );
}


void PlaylistWidget::ShowContextMenu( const QPoint& pos )
{
    if( lwMain->count() > 0 && lwMain->currentRow() >= 0 )
    {
        QMenu menu;
        menu.addAction( QIcon( ":/tool/delete" ), tr( "Remove from playlist" ), this, SLOT( RemoveCurrentFromList() ) );
        menu.addAction( QIcon( ":/tool/clear" ),  tr( "Clear all" ),            this, SLOT( Clear() ) );
        menu.exec( mapToGlobal( pos ) );
    }
}


void PlaylistWidget::SelectClickedItem( const QModelIndex& index )
{
    emit ItemSelected( index.data().toString() );
}


void PlaylistWidget::RemoveCurrentFromList()
{
    lwMain->model()->removeRow( lwMain->currentRow() );

    if( lwMain->count() == 0 )
    {
        emit Cleared();
    }
}

