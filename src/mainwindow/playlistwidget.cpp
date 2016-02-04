/*************************************************************************************************
 *                                                                                                *
 *  file: playlistwidget.cpp                                                                      *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>                                     *
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
#include "tools/debug.h"

#include <QAction>
#include <QMenu>

PlayListWidget::PlayListWidget( QWidget* parent ) : QListWidget( parent )
{
    setAlternatingRowColors( true );
    setContextMenuPolicy( Qt::CustomContextMenu );
    setDragDropMode( QAbstractItemView::InternalMove );

    connect( this, &PlayListWidget::customContextMenuRequested, this, &PlayListWidget::ShowContextMenu );
}

void PlayListWidget::AddItem( const QString& title , const QString& filePath )
{
    DebugPrintFunc( "PlayListWidget::AddItem", title );

    QListWidgetItem* item = new QListWidgetItem( title, this );
    item->setToolTip( filePath ); // TODO: setData()
    addItem( item );
    setCurrentItem( item );
}

void PlayListWidget::Clear()
{
    DebugPrintFunc( "PlayListWidget::Clear" );

    clear();
    emit Cleared();
}

QStringList PlayListWidget::GetPathes() const
{
    QStringList pathes;

    for( int i = 0; i < count(); i++ )
    {
        pathes.append( item( i )->toolTip() );
    }

    return( pathes );
}

void PlayListWidget::ShowContextMenu( const QPoint& pos )
{
    if( count() > 0 && currentRow() >= 0 )
    {
        QMenu menu;
        menu.addAction( QIcon( ":/tool/delete" ), tr( "Remove from playlist" ), this, SLOT( RemoveFromList() ) );
        menu.addAction( QIcon( ":/tool/clear" ), tr( "Clear all" ), this, SLOT( Clear() ) );
        menu.exec( mapToGlobal( pos ) );
    }
}

void PlayListWidget::RemoveFromList()
{
    model()->removeRow( currentRow() );

    if( count() == 0 )
    {
        emit Cleared();
    }
}
