/*************************************************************************************************
 *                                                                                                *
 *  file: tablelistmodel.h                                                                        *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2015 Eugene Melnik <jeka7js@gmail.com>                                          *
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

#ifndef TABLELISTMODEL_H
#define TABLELISTMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QStringList>

class TableListModel : public QStandardItemModel
{
    Q_OBJECT

    public:
        TableListModel( QObject* parent = nullptr );

        enum Columns {
            ViewedColumn,
            FavouriteColumn,
            TitleColumn,
            YearColumn,
            GenreColumn,
            DirectorColumn,
            RatingColumn
        };

    private:
        const QStringList colNames = {
            tr( "V" ),  // Is viewed
            tr( "F" ),  // Is favourite
            tr( "Title" ),
            tr( "Year" ),
            tr( "Genre" ),
            tr( "Director" ),
            tr( "Rating" )
        };

        const QStringList colTooltips = {
            tr( "Is viewed" ),
            tr( "Is favourite" ),
            tr( "Title" ),
            tr( "Year" ),
            tr( "Genre" ),
            tr( "Director" ),
            tr( "Rating" )
        };
};

#endif // TABLELISTMODEL_H
