/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewgridmodel.h                                                                    *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>                                     *
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

#ifndef FILMSVIEWGRIDMODEL_H
#define FILMSVIEWGRIDMODEL_H

#include <QAbstractListModel>
#include <QColor>
#include <QList>
#include <QPixmap>
#include <QString>

#include "alexandrasettings.h"
#include "film.h"

class FilmViewGridModel : public QAbstractListModel
{
    Q_OBJECT

    public:
        FilmViewGridModel( AlexandraSettings* s, QObject* parent = nullptr );

        void AppendItem( const Film& film, QColor background );
        void SetItem( int n, const Film& film, QColor background );
        void Clear();
        void RemoveRow( int row );

        int GetItemIndexByTitle( const QString& title );
        QString GetItemTitle( int n );

        int rowCount( const QModelIndex& parent = QModelIndex() ) const;
        QVariant data( const QModelIndex& index, int role ) const;

    private:
        AlexandraSettings* settings = nullptr;

        QList<QString> itemTitle;
        QList<QPixmap> itemImage;
        QList<QString> itemToolTip;
        QList<QColor>  itemBackground;
};

#endif // FILMSVIEWGRIDMODEL_H
