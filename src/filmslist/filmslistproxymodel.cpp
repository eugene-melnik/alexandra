/*************************************************************************************************
 *                                                                                                *
 *  file: filmslistproxymodel.cpp                                                                 *
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

#include "filmslistproxymodel.h"
#include "filmslistmodel.h"

#include <QPainter>


FilmsListProxyModel::FilmsListProxyModel( QObject* parent ) : QSortFilterProxyModel( parent )
{
    pixmapIsViewed = QPixmap( ":/action/viewed" ).scaledToHeight( scaleHeight, Qt::SmoothTransformation );
    pixmapIsFavourite = QPixmap( ":/action/favourite" ).scaledToHeight( scaleHeight, Qt::SmoothTransformation );

    QPixmap zeroRating = QPixmap( ":/info/rating-zero" ).scaledToHeight( scaleHeight, Qt::SmoothTransformation );
    mapPixRatings.insert( 1.0, zeroRating );
    QPixmap fullRating = QPixmap( ":/info/rating-full" ).scaledToHeight( scaleHeight, Qt::SmoothTransformation );
    mapPixRatings.insert( 10.0, fullRating );
}


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
                switch( column )
                {
                    case FilmItem::IsViewedColumn :
                    {
                        if( item->GetIsFilmViewed() )
                        {
                            return( pixmapIsViewed );
                        }

                        break;
                    }

                    case FilmItem::IsFavouriteColumn :
                    {
                        if( item->GetIsFilmFavourite() )
                        {
                            return( pixmapIsFavourite );
                        }

                        break;
                    }

                    case FilmItem::RatingColumn :
                    {
                        double rating = item->GetColumnData( FilmItem::RatingColumn ).toDouble();
                        rating = (rating > 0) ? rating : 1.0;

                        if( !mapPixRatings.contains(rating) )
                        {
                            QPixmap overlay = mapPixRatings.value( 10.0 );
                            int copyWidth = overlay.width() * rating / 10.0;
                            overlay = overlay.copy( 0, 0, copyWidth, overlay.height() );

                            QPixmap base = mapPixRatings.value( 1.0 );
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

