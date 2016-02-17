/*************************************************************************************************
 *                                                                                                *
 *  file: advancedsearchproxymodel.h                                                              *
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

#ifndef ADVANCEDSEARCHPROXYMODEL_H
#define ADVANCEDSEARCHPROXYMODEL_H


#include "filmslist/filmslistproxymodel.h"


#include <QMap>
#include <QStringList>


class AdvancedSearchProxyModel : public FilmsListProxyModel
{
    Q_OBJECT

    public:
        explicit AdvancedSearchProxyModel( QObject* parent = nullptr ) : FilmsListProxyModel(parent) {}

        void SetHideViewed( bool hide = true ) { hideViewed = hide; }
        void SetHideFavourite( bool hide = true ) { hideFavourite = hide; }

        void AddColumnFilter( int column, QStringList words ) { filterColumns.insert( column, words ); }

        void ApplyFilter() { invalidateFilter(); }

    protected:
        bool filterAcceptsRow( int sourceRow, const QModelIndex& sourceParent ) const override
        {
            if( filterColumns.empty() ) return( false );

            FilmItem* film = static_cast<FilmItem*>( sourceModel()->index( sourceRow, 0, sourceParent ).internalPointer() );

            if( hideViewed && film->GetIsFilmViewed() ) return( false );
            if( hideFavourite && film->GetIsFilmFavourite() ) return( false );

            for( int column : filterColumns.keys() )
            {
                QString columnData = film->GetColumnData( column ).toString();
                QStringList words = filterColumns.value( column );
                bool isContains = false;

                for( QString word : words )
                {
                    if( columnData.contains( word, Qt::CaseInsensitive ) )
                    {
                        isContains = true;
                        break;
                    }
                }

                if( !isContains )
                {
//                    if( column == FilmItem::TitleColumn && (filterColumns.contains(FilmItem::DescriptionColumn) ||
//                                                            filterColumns.contains(FilmItem::TaglineColumn)) )
//                    {
//                        continue;
//                    }
//                    else if( column == FilmItem::DescriptionColumn || column == FilmItem::TaglineColumn )
//                    {
//                        continue;
//                    }
//                    else
                    {
                        return( false );
                    }
                }
            }

            return( true );
        }

    private:
        bool hideViewed = false;
        bool hideFavourite = false;
        QMap<int,QStringList> filterColumns;
};


#endif // ADVANCEDSEARCHPROXYMODEL_H

