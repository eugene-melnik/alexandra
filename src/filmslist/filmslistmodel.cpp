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
#include "filmslistloader.h"
#include "old/filmslistoldloader.h"
#include "tools/debug.h"
#include "version.h"

#include <QFileInfo>
#include <QPixmap>


FilmsListModel::FilmsListModel( QObject* parent ) : QAbstractItemModel( parent ),
      rootItem( nullptr ), settings( AlexandraSettings::GetInstance() )
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

    connect( this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(DatabaseChanged()) );
    connect( this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(DatabaseChanged()) );
    connect( this, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(DatabaseChanged()) );
    connect( this, SIGNAL(layoutChanged()), this, SIGNAL(DatabaseChanged()) );
    connect( this, SIGNAL(modelReset()), this, SIGNAL(DatabaseChanged()) );

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

    return( parentItem->GetChildrenCount() );
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
                if( settings->GetCheckFilesOnStartup() && !item->GetIsFileExists() )
                {
                    return( QColor( settings->GetUnavailableFileColor() ) );
                }
            }

            case Qt::DecorationRole :
            {
                if( column == FilmItem::PosterColumn && item->GetIsPosterExists() )
                {
                    QPixmap pixmap;

                    if( pixmap.load( item->GetPosterFilePath() ) )
                    {
                        return( pixmap );
                    }
                }
            }

            case UserRoles::StringListRole :
            {
                if( column == FilmItem::GenreColumn ||
                    column == FilmItem::CountryColumn ||
                    column == FilmItem::DirectorColumn ||
                    column == FilmItem::ScreenwriterColumn ||
                    column == FilmItem::StarringColumn ||
                    column == FilmItem::TagsColumn )
                {
                    QString str = item->GetColumnData( column ).toString();

                    if( str.isEmpty() )
                    {
                        return( QStringList() );
                    }
                    else
                    {
                        QStringList strings = str.split( "," );

                        for( QString& s : strings )
                        {
                            s = s.trimmed();
                        }

                        return( strings );
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
    DebugPrintFunc( "FilmsList::LoadFromFile", fileName );

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
                isLoaded = FilmsListLoader::Populate( rootItem, fileName );
            }
            else
            {
                emit DatabaseConvertOld();
                isLoaded = FilmsListOldLoader::Populate( rootItem, fileName );

                if( isLoaded )
                {
                    FilmsListLoader::Save( rootItem, fileName + ".json" );
                }
            }

            if( isLoaded )
            {
                DebugPrint( "Reading done!" );
                emit DatabaseLoaded();

                if( rootItem->GetChildrenCount() == 0 )
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
                emit DatabaseReadError( tr( "Can't load database!" ) );
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


void FilmsListModel::AddFilmItem( FilmItem* film )
{
    int pos = rootItem->GetChildrenCount();
    beginInsertRows( QModelIndex(), pos, pos );
    rootItem->AppendChild( film );
    endInsertRows();
}


void FilmsListModel::EditFilmItem( FilmItem* film, const QModelIndex& index )
{
    int row = index.row();
    film->SetParent( rootItem );

    FilmItem* oldFilm = static_cast<FilmItem*>( index.internalPointer() );
    *oldFilm = *film;
    delete film;

    dataChanged( this->index( row, 0 ), this->index( row, FilmItem::ColumnCount - 1 ) );
}


void FilmsListModel::RemoveFilmByIndex( const QModelIndex& index )
{
    if( index.isValid() )
    {
        beginResetModel(); // FIXME: beginRemoveRows should be better, but doesn't work
        FilmItem* item = static_cast<FilmItem*>( index.internalPointer() );

        if( item->GetIsPosterExists() )
        {
            QString posterFileName = settings->GetPostersDirPath() + "/" + item->GetFileName();
            QFile( posterFileName ).remove();
        }

        rootItem->RemoveChild( item );
        endResetModel();

        if( GetIsEmpty() )
        {
            emit DatabaseIsEmpty();
        }
    }
    else
    {
        DebugPrint( "Trying to remove film by invalid index." );
    }
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


QModelIndex FilmsListModel::GetFilmIndex( const QString& title )
{
    for( int i = 0; i < rootItem->GetChildrenCount(); ++i )
    {
        FilmItem* film = rootItem->GetChild(i);

        if( title == film->GetTitle() )
        {
            return( index( i, 0 ) );
        }
    }

    return( QModelIndex() );
}


int FilmsListModel::GetCountOf( FilmItem::Column column, const QVariant& data ) const
{
    int counter = 0;

    for( int i = 0; i < rootItem->GetChildrenCount(); ++i )
    {
        const FilmItem* film = rootItem->GetChild( i );

        if( film->GetColumnData( column ) == data )
        {
            ++counter;
        }
    }

    return( counter );
}

