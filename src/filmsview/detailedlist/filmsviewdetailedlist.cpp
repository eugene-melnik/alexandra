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
#include "filmdetailedinfo.h"
#include "alexandrasettings.h"
#include "filmslist/filmslistmodel.h"
#include "tools/debug.h"

#include <QHeaderView>


FilmsViewDetailedList::FilmsViewDetailedList( QWidget* parent ) : QTableWidget( parent )
{
      // Appearance
    setAlternatingRowColors( true );
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
    setEditTriggers( QAbstractItemView::NoEditTriggers );
    setContextMenuPolicy( Qt::CustomContextMenu );
    setShowGrid( false );

    horizontalHeader()->setStretchLastSection( true );
    horizontalHeader()->hide();
    verticalHeader()->hide();

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


void FilmsViewDetailedList::SetModel( FilmsListProxyModel* model )
{
    sourceModel = model;
    ResetContents();

    connect( sourceModel, &FilmsListProxyModel::modelReset, this, &FilmsViewDetailedList::ResetContents);

//    connect( selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SIGNAL(CurrentChanged(QModelIndex)) );

//    connect( sourceModel, &QAbstractItemModel::dataChanged, this, [this]
//    {
//        QModelIndex index = currentIndex();
//        selectionModel()->currentChanged( index, index );
//    } );
}


void FilmsViewDetailedList::ResetContents()
{
    DebugPrintFunc( "FilmsViewDetailedList::ResetContents" );

    clearContents();
    setColumnCount( 1 );
    setRowCount( sourceModel->rowCount() );

    for( int row = 0; row < sourceModel->rowCount(); ++row )
    {
        QModelIndex index = sourceModel->index( row, FilmItem::TitleColumn );
        const FilmItem* film = sourceModel->GetFilmItemByIndex( index );

        FilmDetailedInfo* filmInfoWidget = new FilmDetailedInfo( this );
        filmInfoWidget->SetLazyLoading( true );
        filmInfoWidget->SetFilm( film );

        setCellWidget( row, 0, filmInfoWidget );
        setRowHeight( row, 150 );
    }

    DebugPrintFuncDone( "FilmsViewDetailedList::ResetContents" );
}

