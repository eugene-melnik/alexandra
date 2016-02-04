/*************************************************************************************************
 *                                                                                                *
 *  file: filmposterview.cpp                                                                      *
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

#include "filmposterview.h"
#include "filmslist/filmitem.h"
#include "tools/debug.h"

#include <QAbstractProxyModel>
#include <QResizeEvent>


void FilmPosterView::ShowInformation( const QModelIndex& index )
{
    DebugPrintFunc( "FilmPosterView::ShowInformation", index.row() );

    const QAbstractProxyModel* model = static_cast<const QAbstractProxyModel*>( index.model() );
    QVariant data = model->index( index.row(), FilmItem::PosterColumn ).data( Qt::DecorationRole );

    if( data.isValid() )
    {
        SetPixmap( data.value<QPixmap>() );
    }
    else
    {
        Clear();
    }

    DebugPrintFuncDone( "FilmPosterView::ShowInformation" );
}


void FilmPosterView::Clear()
{
    SetPixmap( QPixmap( ":/standart-poster" ) );
}


void FilmPosterView::resizeEvent( QResizeEvent* event )
{
    QLabel::resizeEvent( event );

    if( event->oldSize().width() != event->size().width() &&
        pixmap() != nullptr )
    {
        SetPixmap( *pixmap() );
    }
}


void FilmPosterView::showEvent( QShowEvent* event )
{
    QLabel::showEvent( event );
    Clear();
}


void FilmPosterView::SetPixmap( const QPixmap& pixmap )
{
    QPixmap p = pixmap.scaledToWidth( width(), Qt::SmoothTransformation );
    setMinimumHeight( p.height() );
    setPixmap( p );
}

