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


QVariant FilmsListProxyModel::data( const QModelIndex& index, int role ) const
{
    if( index.isValid() )
    {
        switch( role )
        {
            case Qt::DisplayRole :
            {
                const FilmItem* item = GetFilmItemByIndex( index );
                const int column = index.column();

                switch( column )
                {
                    case FilmItem::IsViewedColumn :
                    case FilmItem::IsFavouriteColumn :
                    {
                        return( item->GetColumnData(column).toBool() ? "+" : "-" );
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

                    case FilmItem::RatingColumn :
                    {
                        double rating = item->GetColumnData( FilmItem::RatingColumn ).toDouble();

                        if( rating > 0.1 )
                        {
                            return( QString( "%L1/10" ).arg( rating, 0, 'f', 1 ) );
                        }
                        else
                        {
                            return( QVariant() );
                        }
                    }

                    case FilmItem::BudgetColumn :
                    {
                        double budget = item->GetColumnData( FilmItem::BudgetColumn ).toDouble();

                        if( budget > 1 )
                        {
                            return( QString( "$%L1" ).arg( budget, 0, 'f', 0 ) );
                        }
                        else
                        {
                            return( QVariant() );
                        }
                    }
                }
            }
        }
    }

    return( QSortFilterProxyModel::data( index, role ) );
}

