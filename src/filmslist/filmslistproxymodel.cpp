/*************************************************************************************************
 *                                                                                                *
 *  file: filmslistproxymodel.cpp                                                                 *
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

#include "filmslistproxymodel.h"
#include "filmslistmodel.h"

#include <QPainter>


QVariant FilmsListProxyModel::data( const QModelIndex& index, int role ) const
{
    if( index.isValid() )
    {
        const FilmItem* item = GetFilmItemByIndex( index );
        const int column = index.column();

        switch( role )
        {
            case Qt::DisplayRole :
            {
                switch( column )
                {
                    case FilmItem::IsViewedColumn :
                    case FilmItem::IsFavouriteColumn :
                    case FilmItem::RatingColumn :
                    {
                        return( QVariant() );
                    }

                    case FilmItem::YearColumn :
                    {
                        int year = item->GetColumnData(column).toInt();

                        if( year > 1849 ) // See at "addfilmwindow.ui" file "sbYear" widget
                        {
                            return( year );
                        }
                        else
                        {
                            return( QVariant() );
                        }
                    }

                    case FilmItem::BudgetColumn :
                    {
                        double budget = item->GetColumnData( FilmItem::BudgetColumn ).toDouble();

                        if( budget > 1.0 )
                        {
                            return( QString( "$%L1" ).arg( budget, 0, 'f', 0 ) );
                        }
                        else
                        {
                            return( QVariant() );
                        }
                    }
                }

                break;
            }

            case Qt::DecorationRole :
            {
                int scaleHeight = 10;

                switch( column )
                {
                    case FilmItem::IsViewedColumn :
                    {
                        if( item->GetIsFilmViewed() )
                        {
                            if( pixmapIsViewed.isNull() )
                            {
                                pixmapIsViewed = QPixmap( ":/action/viewed" ).scaledToHeight( scaleHeight, Qt::SmoothTransformation );
                            }

                            return( pixmapIsViewed );
                        }

                        break;
                    }

                    case FilmItem::IsFavouriteColumn :
                    {
                        if( item->GetIsFilmFavourite() )
                        {
                            if( pixmapIsFavourite.isNull() )
                            {
                                pixmapIsFavourite = QPixmap( ":/action/favourite" ).scaledToHeight( scaleHeight, Qt::SmoothTransformation );
                            }

                            return( pixmapIsFavourite );
                        }

                        break;
                    }

                    case FilmItem::RatingColumn :
                    {
                        double rating = item->GetColumnData( FilmItem::RatingColumn ).toDouble();

                        if( !mapPixRatings.contains(rating) )
                        {
                            QPixmap overlay = QPixmap( ":/info/rating-full" ).scaledToHeight( scaleHeight, Qt::SmoothTransformation );
                            int copyWidth = overlay.width() * rating / 10.0;
                            overlay = overlay.copy( 0, 0, copyWidth, overlay.height() );

                            QPixmap base = QPixmap( ":/info/rating-zero" ).scaledToHeight( scaleHeight, Qt::SmoothTransformation );
                            QPainter painter( &base );
                            painter.drawPixmap( 0, 0, overlay );

                            mapPixRatings.insert( rating, base );
                        }

                        return( mapPixRatings.value(rating) );
                    }
                }

                break;
            }

            case Qt::ToolTipRole :
            {
                if( column == FilmItem::RatingColumn )
                {
                    double rating = item->GetColumnData( FilmItem::RatingColumn ).toDouble();

                    if( rating > 1.0 )
                    {
                        return( QString( "%L1/10" ).arg( rating, 0, 'f', 2 ) );
                    }
                    else
                    {
                        return( QVariant() );
                    }
                }

                break;
            }
        }
    }

    return( QSortFilterProxyModel::data( index, role ) );
}

