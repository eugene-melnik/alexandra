/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewgridproxymodel.h                                                               *
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

#ifndef FILMSVIEWGRIDPROXYMODEL_H
#define FILMSVIEWGRIDPROXYMODEL_H


#include <QCache>
#include <QPixmap>
#include <QIdentityProxyModel>


#include "alexandrasettings.h"


class FilmsViewGridProxyModel : public QIdentityProxyModel
{
    Q_OBJECT

    public:
        explicit FilmsViewGridProxyModel( QObject* parent = nullptr )
          : QIdentityProxyModel( parent ),
            settings( AlexandraSettings::GetInstance() ),
            pixmapCache( new QCache<QString,QPixmap>() ) {}

        ~FilmsViewGridProxyModel() { delete pixmapCache; }

        void SetCacheSize( int size ) { pixmapCache->setMaxCost( size + 1 ); }

        int columnCount( const QModelIndex& ) const override { return( 1 ); }
        QVariant data( const QModelIndex& index, int role ) const override;

    private:
        AlexandraSettings* settings;
        QCache<QString,QPixmap>* pixmapCache;
};


#endif // FILMSVIEWGRIDPROXYMODEL_H

