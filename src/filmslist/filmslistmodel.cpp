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
#include "tools/debug.h"
#include "version.h"

#include <QtAlgorithms>
#include <QFile>
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

    rootItem = new TreeItem( titles );
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

    TreeItem* parentItem = rootItem;

    if( parent.isValid() )
    {
        parentItem = static_cast<TreeItem*>( parent.internalPointer() );
    }

    return( parentItem->GetChildCount() );
}


int FilmsListModel::columnCount( const QModelIndex& parent ) const
{
    if( parent.isValid() )
    {
        return( static_cast<TreeItem*>( parent.internalPointer() )->GetColumnCount() );
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
                return( rootItem->GetData( section ) );
            }

            case Qt::TextAlignmentRole :
            {
                if( section == IsViewedColumn ||
                    section == IsFavouriteColumn ||
                    section == ViewsCountColumn )
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
        TreeItem* item = static_cast<TreeItem*>( index.internalPointer() );
        int column = index.column();

        switch( role )
        {
            case Qt::DisplayRole :
            {
                return( item->GetData( index.column() ) );
            }

            case Qt::TextAlignmentRole :
            {
                if( column == YearColumn ||
                    column == RatingColumn ||
                    column == IsViewedColumn ||
                    column == IsFavouriteColumn ||
                    column == ViewsCountColumn )
                {
                    return( (int) Qt::AlignCenter );
                }
                else
                {
                    return( (int) Qt::AlignLeft|Qt::AlignVCenter );
                }
            }

            case Qt::BackgroundColorRole :
            {
                bool highlightUnavailable = settings->GetCheckFilesOnStartup();

                if( highlightUnavailable && !QFile::exists( item->GetData( FileNameColumn ).toString() ) )
                {
                    return( QColor(settings->GetUnavailableFileColor()) );
                }
            }

            case Qt::DecorationRole :
            {
                if( column == PosterColumn )
                {
                    QString posterFileName = item->GetData( PosterColumn ).toString();
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
        TreeItem* parentItem = rootItem;

        if( parent.isValid() )
        {
            parentItem = static_cast<TreeItem*>( parent.internalPointer() );
        }

        TreeItem* childItem = parentItem->GetChild( row );

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
        TreeItem* parentItem = static_cast<TreeItem*>( index.internalPointer() )->GetParent();

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
        endResetModel();
        return;
    }

    if( file.open( QIODevice::ReadWrite ) )
    {
        QDataStream stream( &file );
        QString databaseHeader;
        quint8 databaseVersion;
        QList<Film> films;

        stream >> databaseHeader;
        stream >> databaseVersion;

        if( databaseHeader == Alexandra::databaseHeader
            && databaseVersion == Alexandra::databaseVersion )
        {
            stream >> films; // TODO: need to do something
        }

        DebugPrint( "Reading done!" );
        emit DatabaseLoaded();

        if( films.isEmpty() )
        {
            emit DatabaseIsEmpty();
        }
        else
        {
            for( Film& film : films )
            {
                QList<QVariant> data;

                data << film.GetTitle()
                     << film.GetOriginalTitle()
                     << film.GetTagline()
                     << film.GetYearStr()
                     << film.GetGenre()
                     << film.GetCountry()
                     << film.GetDirector()
                     << film.GetProducer()
                     << film.GetScreenwriter()
                     << film.GetComposer()
                     << film.GetBudgetStr()
                     << film.GetRatingStr()
                     << film.GetIsViewedSign()
                     << film.GetIsFavouriteSign()
                     << film.GetViewsCounter()
                     << film.GetStarring()
                     << film.GetDescription()
                     << film.GetTags()
                     << film.GetFileName()
                     << film.GetPosterName();

                TreeItem* item = new TreeItem( data, rootItem );
                rootItem->AppendChild( item );
            }
        }

        if( !file.isWritable() )
        {
            emit DatabaseIsReadonly();
        }

        file.close();
    }
    else
    {
        DebugPrint( "Reading failed!" );
        emit DatabaseReadError();
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

