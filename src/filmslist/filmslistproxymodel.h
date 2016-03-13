/*************************************************************************************************
 *                                                                                                *
 *  file: filmslistproxymodel.h                                                                   *
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

#ifndef FILMSLISTPROXYMODEL_H
#define FILMSLISTPROXYMODEL_H


#include <QList>
#include <QPixmap>
#include <QSortFilterProxyModel>


#include "filmslist/filmitem.h"


class FilmsListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    public:
        explicit FilmsListProxyModel( QObject* parent = nullptr );

        QVariant data( const QModelIndex &index, int role ) const override;

    public slots:
        const FilmItem* GetFilmItemByIndex( const QModelIndex& index ) const
        {
            return( static_cast<FilmItem*>( mapToSource(index).internalPointer() ) );
        }

        QString GetFilmTitleByIndex( const QModelIndex& index ) const
        {
            const FilmItem* film = GetFilmItemByIndex(index);

            if( film != nullptr )
            {
                return( film->GetTitle() );
            }
            else
            {
                return( QString() );
            }
        }

        QString GetFilmFileNameByIndex( const QModelIndex& index ) const
        {
            const FilmItem* film = GetFilmItemByIndex(index);

            if( film != nullptr )
            {
                return( film->GetFileName() );
            }
            else
            {
                return( QString() );
            }
        }

        void SetFilter( const QString& string, const QList<int>& columns )
        {
            filterString = string;
            filterColumns = columns;
            invalidateFilter();
        }

        void SetShowViewed( bool show ) { hideViewed = !show; invalidateFilter(); }
        void SetShowFavourite( bool show ) { hideFavourite = !show; invalidateFilter(); }
        void SetHideUnavailable( bool hide ) { hideUnavailable = hide; invalidateFilter(); }

    protected:
        bool filterAcceptsRow( int sourceRow, const QModelIndex& sourceParent ) const override
        {
            const FilmItem* film = static_cast<FilmItem*>( sourceModel()->index( sourceRow, 0, sourceParent ).internalPointer() );

            if( hideViewed && film->GetIsFilmViewed() ) return( false );
            if( hideFavourite && film->GetIsFilmFavourite() ) return( false );
            if( hideUnavailable && !film->GetIsFileExists() ) return( false );

            if( filterString.isEmpty() ) return( true );
            if( filterColumns.isEmpty() ) return( false );

            for( int column : filterColumns )
            {
                QString columnString = film->GetColumnData( column ).toString();
                if( columnString.contains( filterString, Qt::CaseInsensitive ) ) return( true );
            }

            return( false );
        }

    private:
        QList<int> filterColumns;
        QString filterString;

        bool hideViewed = false;
        bool hideFavourite = false;
        bool hideUnavailable = false;

        mutable QPixmap pixmapIsViewed;
        mutable QPixmap pixmapIsFavourite;
        mutable QMap<double,QPixmap> mapPixRatings;
        const int scaleHeight = 10;
};


#endif // FILMSLISTPROXYMODEL_H

