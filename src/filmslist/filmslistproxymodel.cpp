
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

                        if( rating > 0.5 )
                        {
                            return( QString( "%1/10" ).arg( rating, 0, 'f', 1 ) );
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

