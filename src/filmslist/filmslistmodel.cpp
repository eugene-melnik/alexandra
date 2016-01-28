/*************************************************************************************************
 *                                                                                                *
 *  file: filmslistmodel.cpp                                                                      *
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

#include "filmslistmodel.h"
#include "old/filmslistoldloader.h"
#include "tools/debug.h"
#include "version.h"

#include <QFileInfo>
#include <QPixmap>


FilmsListModel::FilmsListModel( QObject* parent ) : QAbstractItemModel( parent ),
      rootItem( nullptr ), isDatabaseChanged( false ), settings( AlexandraSettings::GetInstance() )
{
    DebugPrintFunc( "FilmsListModel::FilmsListModel" );

    QList<QVariant> titles =
    {
        tr( "Title" ),
        tr( "Original title" ),
        tr( "Tagline" ),
        tr( "Year" ),
        tr( "Genre" ),
        tr( "Country" ),
        tr( "Director" ),
        tr( "Producer" ),
        tr( "Screenwriter" ),
        tr( "Composer" ),
        tr( "Budget" ),
        tr( "Rating" ),
        tr( "Viewed" ),
        tr( "Favourite" ),
        tr( "Views" ),
        tr( "Starring" ),
        tr( "Description" ),
        tr( "Tags" ),
        tr( "Filename" ),
        tr( "Poster" )
    };

    rootItem = new FilmItem( titles );
    DebugPrintFuncDone( "FilmsListModel::FilmsListModel" );
}


FilmsListModel::~FilmsListModel()
{
      // Behavior on program exit while data saving
    if( mxAsyncSaveToFile.tryLock( 5000 ) ) // 5 sec
    {
        mxAsyncSaveToFile.unlock();
    }

    delete rootItem;
}


int FilmsListModel::rowCount( const QModelIndex& parent ) const
{
    if( parent.column() > 0 )
    {
        return( 0 );
    }

    FilmItem* parentItem = rootItem;

    if( parent.isValid() )
    {
        parentItem = static_cast<FilmItem*>( parent.internalPointer() );
    }

    return( parentItem->GetChildCount() );
}


int FilmsListModel::columnCount( const QModelIndex& parent ) const
{
    if( parent.isValid() )
    {
        return( static_cast<FilmItem*>( parent.internalPointer() )->GetColumnCount() );
    }
    else
    {
        return( rootItem->GetColumnCount() );
    }
}


Qt::ItemFlags FilmsListModel::flags( const QModelIndex& index ) const
{
    if( index.isValid() )
    {
        return( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    }

    return( 0 );
}


QVariant FilmsListModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( orientation == Qt::Horizontal )
    {
        switch( role )
        {
            case Qt::DisplayRole :
            case Qt::ToolTipRole :
            {
                return( rootItem->GetColumnData( section ) );
            }

            case Qt::TextAlignmentRole :
            {
                if( section == FilmItem::IsViewedColumn ||
                    section == FilmItem::IsFavouriteColumn ||
                    section == FilmItem::ViewsCountColumn )
                {
                    return( Qt::AlignLeft );
                }
            }
        }
    }

    return( QVariant() );
}


QVariant FilmsListModel::data( const QModelIndex& index, int role ) const
{
    if( index.isValid() )
    {
        FilmItem* item = static_cast<FilmItem*>( index.internalPointer() );
        int column = index.column();

        switch( role )
        {
            case Qt::DisplayRole :
            {
                return( item->GetColumnData( index.column() ) );
            }

            case Qt::TextAlignmentRole :
            {
                if( column == FilmItem::YearColumn ||
                    column == FilmItem::RatingColumn ||
                    column == FilmItem::IsViewedColumn ||
                    column == FilmItem::IsFavouriteColumn ||
                    column == FilmItem::ViewsCountColumn )
                {
                    return( Qt::AlignCenter );
                }
                else
                {
                    return( (int) Qt::AlignLeft | Qt::AlignVCenter );
                }
            }

            case Qt::BackgroundColorRole :
            {
                if( settings->GetCheckFilesOnStartup() )
                {
                    QColor color = item->GetHighlightColor();

                    if( color.isValid() )
                    {
                        return( color );
                    }
                    else if( !QFile::exists( item->GetColumnData( FilmItem::FileNameColumn ).toString() ) )
                    {
                        item->SetHighlightColor( settings->GetUnavailableFileColor() );
                        return( item->GetHighlightColor() );
                    }
                }
            }

            case Qt::DecorationRole :
            {
                if( column == FilmItem::PosterColumn )
                {
                    QString posterFileName = item->GetColumnData( FilmItem::PosterColumn ).toString();
                    QString posterFilePath = settings->GetPostersDirPath() + "/" + posterFileName;
                    QPixmap pixmap;

                    if( pixmap.load( posterFilePath ) )
                    {
                        return( pixmap );
                    }
                }
            }
        }
    }

    return( QVariant() );
}


QModelIndex FilmsListModel::index( int row, int column, const QModelIndex& parent ) const
{
    if( hasIndex( row, column, parent ) )
    {
        FilmItem* parentItem = rootItem;

        if( parent.isValid() )
        {
            parentItem = static_cast<FilmItem*>( parent.internalPointer() );
        }

        FilmItem* childItem = parentItem->GetChild( row );

        if( childItem != nullptr )
        {
            return( createIndex( row, column, childItem ) );
        }
    }

    return( QModelIndex() );
}


QModelIndex FilmsListModel::parent( const QModelIndex& index ) const
{
    if( index.isValid() )
    {
        FilmItem* parentItem = static_cast<FilmItem*>( index.internalPointer() )->GetParent();

        if( parentItem != rootItem )
        {
            return createIndex( parentItem->GetRow(), 0, parentItem );
        }
    }

    return QModelIndex();
}


void FilmsListModel::LoadFromFile( const QString& fileName )
{
    DebugPrintFuncA( "FilmsList::LoadFromFile", fileName );

    beginResetModel();
    rootItem->RemoveChildren();
    QFile file( fileName );

    if( !file.exists() )
    {
        emit DatabaseIsEmpty();
    }
    else if( file.open( QIODevice::ReadOnly ) )
    {
        QDataStream stream( &file );
        QString databaseHeader;
        stream >> databaseHeader;

        if( databaseHeader == Alexandra::databaseHeader )
        {
            bool isLoaded;
            quint8 databaseVersion;
            stream >> databaseVersion;
            file.close();

            if( databaseVersion == Alexandra::databaseVersion )
            {
                ///
                /// new loading will be here
                ///
            }
            else
            {
                isLoaded = FilmsListOldLoader::Populate( rootItem, fileName );
            }

            if( isLoaded )
            {
                DebugPrint( "Reading done!" );
                emit DatabaseLoaded();

                if( rootItem->GetChildCount() == 0 )
                {
                    emit DatabaseIsEmpty();
                }

                if( !QFileInfo(fileName).isWritable() )
                {
                    emit DatabaseIsReadonly();
                }
            }
            else
            {
                emit DatabaseReadError( tr( "Error while trying to convert old database format to new one." ) );
            }
        }
        else
        {
            emit DatabaseReadError( tr( "Wrong database file format." ) );
        }
    }
    else
    {
        emit DatabaseReadError( tr( "Error while reading database file. Check the access permissions." ) );
    }

    endResetModel();
    DebugPrintFuncDone( "FilmsList::LoadFromFile" );
}


void FilmsListModel::EraseAll()
{
    DebugPrintFunc( "FilmsListModel::EraseAll" );

    beginResetModel();
    rootItem->RemoveChildren();
    endResetModel();

    // TODO: save database

    DebugPrintFuncDone( "FilmsListModel::EraseAll" );
}

