/*************************************************************************************************
 *                                                                                                *
 *  file: filmslistmodel.h                                                                        *
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

#ifndef FILMSLISTMODEL_H
#define FILMSLISTMODEL_H


#include <QAbstractItemModel>
#include <QMutex>
#include <QObject>
#include <QStringList>


#include "alexandrasettings.h"
#include "filmitem.h"


class FilmsListModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit FilmsListModel( QObject* parent = nullptr );
        virtual ~FilmsListModel();

          // Model reading
        int rowCount( const QModelIndex& parent ) const override;
        int columnCount( const QModelIndex& parent ) const override;

        Qt::ItemFlags flags( const QModelIndex& index ) const override;
        QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
        QVariant data( const QModelIndex &index, int role ) const override;

        QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
        QModelIndex parent( const QModelIndex& index ) const override;

          // Model writing
        bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole ) override;

          // Types
        enum UserRoles
        {
            StringListRole = Qt::UserRole
        };

    public slots:
        void LoadFromFile( QString fileName );
        void SaveToFile( QString fileName );
        void SaveToFileAsync( QString fileName );

        void AddFilmItem( FilmItem* film );
        void EditFilmItem( FilmItem* film, const QModelIndex& index );
        void RemoveFilmByIndex( const QModelIndex& index );

        void FilmsMoved();
        void EraseAll();
        void ResetViews();

        bool GetIsEmpty() const { return( GetFilmsCount() == 0 ); }
        int GetFilmsCount() const { return( rootItem->GetChildrenCount() ); }
        int GetIsViewedFilmsCount() const { return( GetCountOf( FilmItem::IsViewedColumn, true ) ); }
        int GetIsFavouriteFilmsCount() const { return( GetCountOf( FilmItem::IsFavouriteColumn, true ) ); }

        QModelIndex GetFilmIndex( QString title ) const;

    signals:
        void DatabaseConvertOld();
        void DatabaseReadError( QString );
        void DatabaseWriteError();
        void DatabaseIsEmpty();
        void DatabaseIsReadonly();

        void DatabaseLoaded();
        void DatabaseChanged();

    private:
        int GetCountOf( FilmItem::Column column, const QVariant& data ) const;

          // Variables
        AlexandraSettings* settings;
        FilmItem* rootItem;
        QMutex mutexDataEdit;
        bool isDatabaseChanged = false;
};


#endif // FILMSLISTMODEL_H

