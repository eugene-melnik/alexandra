/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewgrid.cpp                                                                       *
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

#include "filmsviewgrid.h"

#include <QScrollBar>


FilmsViewGrid::FilmsViewGrid( QWidget* parent ) : QListView( parent ),
    proxyModel( new FilmsViewGridProxyModel( this ) )
{
    QListView::setModel( proxyModel );

      // Appearance
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setEditTriggers( QAbstractItemView::NoEditTriggers );
    setContextMenuPolicy( Qt::CustomContextMenu );
    setResizeMode( QListView::Adjust );
    setViewMode( QListView::IconMode );
    setWrapping( true );
    setSpacing( 5 );

//    setVerticalScrollMode( QAbstractItemView::ScrollPerPixel ); // WTF: Qt bug?

      // Signals
    connect( this, &QListView::customContextMenuRequested, this, [this] (const QPoint& pos)
    {
        if( currentIndex().isValid() )
        {
            emit ContextMenuRequested( pos, proxyModel->mapToSource( currentIndex() ) );
        }
    });
}


void FilmsViewGrid::setModel( QAbstractItemModel* model )
{
    proxyModel->setSourceModel( model );

    connect( selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(MapCurrentChanged(QModelIndex)) );

    connect( model, &QAbstractItemModel::modelReset, this, [this, model]
    {
        proxyModel->SetCacheSize( model->rowCount() );
    } );

    connect( model, &QAbstractItemModel::dataChanged, this, [this]
    {
        QModelIndex index = currentIndex();
        selectionModel()->currentChanged( index, index );
    } );
}


void FilmsViewGrid::ReloadSettings()
{
    setStyleSheet( QString( "font-size: %1pt" ).arg( AlexandraSettings::GetInstance()->GetGridFontSize() ) );
}


QModelIndexList FilmsViewGrid::GetSelectedItemsList()
{
    QModelIndexList indexes = selectedIndexes();
    QModelIndexList mappedIndexes;

    for( const QModelIndex& index : indexes )
    {
        mappedIndexes.append( proxyModel->mapToSource(index) );
    }

    if( mappedIndexes.isEmpty() )
    {
        mappedIndexes.append( currentIndex() );
    }

    return( mappedIndexes );
}


void FilmsViewGrid::updateGeometries()
{
    QListView::updateGeometries();
    verticalScrollBar()->setSingleStep( AlexandraSettings::GetInstance()->GetGridItemSize() / 5 );
}


void FilmsViewGrid::MapCurrentChanged( const QModelIndex& index )
{
    emit CurrentChanged( proxyModel->mapToSource(index) );
}

