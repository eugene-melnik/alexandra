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

        void AddColumnOrFilter( int column, QStringList words ) { filterColumnsOr.insert( column, words ); }
        void AddColumnAndFilter( int column, QStringList words ) { filterColumnsAnd.insert( column, words ); }

        void ApplyFilter() { invalidateFilter(); }

    protected:
        bool filterAcceptsRow( int sourceRow, const QModelIndex& sourceParent ) const override
        {
              // Filter wasn't set
            if( filterColumnsAnd.empty() && filterColumnsOr.isEmpty() ) return( false );

            FilmItem* film = static_cast<FilmItem*>( sourceModel()->index( sourceRow, 0, sourceParent ).internalPointer() );

              // "OR" filter
            if( !filterColumnsOr.isEmpty() )
            {
                bool isContains = false;

                for( int column : filterColumnsOr.keys() )
                {
                    QString columnData = film->GetColumnData( column ).toString();
                    QStringList words = filterColumnsOr.value( column );

                    for( QString word : words )
                    {
                        if( columnData.contains( word, Qt::CaseInsensitive ) )
                        {
                            isContains = true;
                            break;
                        }
                    }

                    if( isContains ) break;
                }

                if( !isContains ) return( false );
            }

              // "AND" filter
            for( int column : filterColumnsAnd.keys() )
            {
                QString columnData = film->GetColumnData( column ).toString();
                QStringList words = filterColumnsAnd.value( column );
                bool isContains = false;

                for( QString word : words )
                {
                    if( columnData.contains( word, Qt::CaseInsensitive ) )
                    {
                        isContains = true;
                        break;
                    }
                }

                if( !isContains ) return( false );
            }

            return( true );
        }

    private:
        QMap<int,QStringList> filterColumnsOr;
        QMap<int,QStringList> filterColumnsAnd;
};


#endif // ADVANCEDSEARCHPROXYMODEL_H

