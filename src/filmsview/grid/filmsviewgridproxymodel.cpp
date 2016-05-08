/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewgridproxymodel.cpp                                                             *
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

#include "filmsviewgridproxymodel.h"
#include "filmslist/filmitem.h"

#include <QPainter>


QVariant FilmsViewGridProxyModel::data( const QModelIndex& index, int role ) const
{
    if( index.isValid() )
    {
        QAbstractItemModel* model = sourceModel();
        int row = index.row();

        switch( role )
        {
            case Qt::TextAlignmentRole :
            {
                return( Qt::AlignCenter );
            }

            case Qt::DecorationRole :
            {
                QString posterFileName = model->index( row, FilmItem::PosterColumn ).data().toString();

                if( !pixmapCache->contains(posterFileName) )
                {
                    QVariant data = model->index( row, FilmItem::PosterColumn ).data( Qt::DecorationRole );
                    QPixmap* p = new QPixmap( data.value<QPixmap>().scaledToHeight( settings->GetGridItemSize(), Qt::SmoothTransformation ) );

                    pixmapCache->insert( posterFileName, p );
                }

                  // Rating stars over poster
                /// TODO: add option in settings
                QPixmap* p = pixmapCache->object( posterFileName );
                QVariant data = model->index( row, FilmItem::RatingColumn ).data( Qt::DecorationRole );
                QPixmap pixmap = data.value<QPixmap>();
                QPainter painter( p );
                painter.drawPixmap( 0, p->height() - pixmap.height(), pixmap );

                return( *p );
            }

            case Qt::ToolTipRole :
            {
                if( settings->GetGridShowTooltip() == true )
                {
                    QString tooltip, title, text;

                    for( int column = 1; column < model->columnCount() - 2; ++column ) // without filenames
                    {
                        text = model->index( row, column ).data().toString();

                        if( text.length() > 1 )
                        {
                            title = model->headerData( column, Qt::Horizontal ).toString();
                            tooltip += QString( "<b>%1:</b> %2<br/>" ).arg( title, text );
                        }
                    }

                    return( tooltip );
                }
            }

            case Qt::SizeHintRole :
            {
                int width = settings->GetGridItemSize() + settings->GetGridItemSize() * 0.1; // +10%
                int height = settings->GetGridItemSize() + settings->GetGridFontSize() * 2;  // +2* font size
                return( QSize( width, height ) );
            }
        }
    }

    return QIdentityProxyModel::data( index, role );
}

