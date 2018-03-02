/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewgrid.cpp                                                                       *
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
    connect( this, SIGNAL(activated(QModelIndex)), this, SIGNAL(CurrentActivated(QModelIndex)) );

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
    QFont font = this->font();
    font.setPointSize(AlexandraSettings::GetInstance()->GetGridFontSize());
    setFont( font );
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

