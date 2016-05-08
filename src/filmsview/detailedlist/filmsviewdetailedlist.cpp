/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewdetailedlist.cpp                                                               *
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

#include "filmsviewdetailedlist.h"
#include "alexandrasettings.h"
#include "filmslist/filmslistmodel.h"
#include "tools/debug.h"

#include <QScrollBar>


FilmsViewDetailedList::FilmsViewDetailedList( QWidget* parent ) : QListView( parent ),
    infoDelegate( new FilmDetailedInfoDelegate() )
{
    setItemDelegate( infoDelegate );

      // Appearance
    setAlternatingRowColors( true );
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setEditTriggers( QAbstractItemView::NoEditTriggers );
    setContextMenuPolicy( Qt::CustomContextMenu );

    setVerticalScrollMode( QAbstractItemView::ScrollPerPixel ); // WTF: Qt bug?

      // Signals
    connect( this, SIGNAL(activated(QModelIndex)), this, SIGNAL(CurrentActivated(QModelIndex)) );

    connect( this, &QWidget::customContextMenuRequested, this, [this] (const QPoint& pos)
    {
        if( GetCurrentIndex().isValid() )
        {
            emit ContextMenuRequested( pos, GetCurrentIndex() );
        }
    } );
}


FilmsViewDetailedList::~FilmsViewDetailedList()
{
    delete infoDelegate;
}


void FilmsViewDetailedList::SetModel( FilmsListProxyModel* model )
{
    QListView::setModel( model );

    connect( selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SIGNAL(CurrentChanged(QModelIndex)) );

    connect( model, &QAbstractItemModel::dataChanged, this, [this]
    {
        QModelIndex index = currentIndex();
        selectionModel()->currentChanged( index, index );
    } );
}


void FilmsViewDetailedList::updateGeometries()
{
    QListView::updateGeometries();
    verticalScrollBar()->setSingleStep( 40 ); /// FIXME: make dependence on item height
}

