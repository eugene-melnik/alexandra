/*************************************************************************************************
 *                                                                                                *
 *  file: filmslistmodel.cpp                                                                      *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2017 Eugene Melnik <eugene@melnik.solutions>                               *
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

#include <thread>
#include <QDir>
#include <QFileInfo>
#include <QPixmap>


FilmsListModel::FilmsListModel( QObject* parent ) : QAbstractItemModel( parent ),
      settings( AlexandraSettings::GetInstance() ), rootItem( nullptr )
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

    connect( &savingTimer, &QTimer::timeout, this, [this]
    {
        SaveToFileAsync( savingFileName );
    } );

    DebugPrintFuncDone( "FilmsListModel::FilmsListModel" );
}


FilmsListModel::~FilmsListModel()
{
      // Behavior on program exit while data saving
    if( mutexDataEdit.tryLock( 5000 ) ) // 5 sec
    {
        mutexDataEdit.unlock();
    }

    if( savingTimer.isActive() )
    {
        savingTimer.stop();
        SaveToFileSync( savingFileName );
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

                break;
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

                break;
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

                    if( !str.isEmpty() )
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


bool FilmsListModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if( index.isValid() && role == Qt::EditRole )
    {
        QMutexLocker locker( &mutexDataEdit );
        isDatabaseChanged = true;

        FilmItem* film = static_cast<FilmItem*>( index.internalPointer() );
        film->SetColumnData( index.column(), value );
        emit dataChanged( index, index );
        return( true );
    }

    return( false );
}


void FilmsListModel::LoadFromFile( QString fileName )
{
    DebugPrintFunc( "FilmsList::LoadFromFile", fileName );

    beginResetModel();
    rootItem->RemoveChildren();
    QFile file( fileName );

    if( !file.exists() )
    {
        isDatabaseChanged = true; // Will be created
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
                    FilmsListLoader::Save( rootItem, fileName );
                }
            }

            if( isLoaded )
            {
                DebugPrint( "Reading done!" );
                emit DatabaseLoaded();
                isReadonly = false;

                if( rootItem->GetChildrenCount() == 0 )
                {
                    emit DatabaseIsEmpty();
                }

                if( !QFileInfo(fileName).isWritable() )
                {
                    emit DatabaseIsReadonly();
                    isReadonly = true;
                }
            }
            else
            {
                emit DatabaseReadError( tr("Can't load database!") );
            }
        }
        else
        {
            emit DatabaseReadError( tr("Wrong database file format.") );
        }
    }
    else
    {
        emit DatabaseReadError( tr("Error while reading database file. Check the access permissions.") );
    }

    endResetModel();
    DebugPrintFuncDone( "FilmsList::LoadFromFile" );
}


void FilmsListModel::SaveToFile( QString fileName )
{
    savingTimer.stop();
    savingFileName = fileName;
    savingTimer.start( 5000 ); // 5 seconds
}


void FilmsListModel::AddFilmItem( FilmItem* film )
{
    QMutexLocker locker( &mutexDataEdit );
    isDatabaseChanged = true;

    int pos = rootItem->GetChildrenCount();
    beginInsertRows( QModelIndex(), pos, pos );

    #ifdef PORTABLE_VERSION
        if (!film->GetFileName().isEmpty())
        {
            QString realativePath = QDir(qApp->applicationDirPath()).relativeFilePath(film->GetFileName());
            film->SetColumnData( FilmItem::FileNameColumn, realativePath);
        }
    #endif

    rootItem->AppendChild( film );
    endInsertRows();
}


void FilmsListModel::EditFilmItem( FilmItem* film, const QModelIndex& index )
{
    QMutexLocker locker( &mutexDataEdit );
    isDatabaseChanged = true;

    int row = index.row();
    film->SetParent( rootItem );

    #ifdef PORTABLE_VERSION
        if (!film->GetFileName().isEmpty())
        {
            QString realativePath = QDir(qApp->applicationDirPath()).relativeFilePath(film->GetFileName());
            film->SetColumnData( FilmItem::FileNameColumn, realativePath);
        }
    #endif

    FilmItem* oldFilm = static_cast<FilmItem*>( index.internalPointer() );
    *oldFilm = *film;
    delete film;

    dataChanged( this->index( row, 0 ), this->index( row, FilmItem::VisibleColumnCount - 1 ) );
}


void FilmsListModel::RemoveFilmByIndex( const QModelIndex& index )
{
    if( index.isValid() )
    {
        QMutexLocker locker( &mutexDataEdit );
        isDatabaseChanged = true;

        beginResetModel(); // FIXME: beginRemoveRows should be better, but doesn't work
        FilmItem* item = static_cast<FilmItem*>( index.internalPointer() );

        if( item->GetIsPosterExists() )
        {
            QFile::remove( item->GetPosterFilePath() );
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


void FilmsListModel::Invalidate()
{
    isDatabaseChanged = true;
    emit DatabaseChanged();
}


void FilmsListModel::EraseAll()
{
    DebugPrintFunc( "FilmsListModel::EraseAll" );

    while( rowCount(QModelIndex()) != 0 )
    {
        RemoveFilmByIndex( index( 0, 0 ) );
    }

    DebugPrintFuncDone( "FilmsListModel::EraseAll" );
}


void FilmsListModel::ResetViews()
{
    QMutexLocker locker( &mutexDataEdit );
    isDatabaseChanged = true;
    beginResetModel();

    for( int row = 0; row < rootItem->GetChildrenCount(); ++row )
    {
        rootItem->GetChild(row)->ResetViewsCounter();
    }

    endResetModel();
}


void FilmsListModel::IncViewsCounterForIndex( const QModelIndex& index )
{
    QMutexLocker locker( &mutexDataEdit );
    isDatabaseChanged = true;

    FilmItem* film = static_cast<FilmItem*>( index.internalPointer() );
    film->IncViewsCounter();
    dataChanged( index, index );
}


void FilmsListModel::ResetViewsCounterForIndex( const QModelIndex& index )
{
    QMutexLocker locker( &mutexDataEdit );
    isDatabaseChanged = true;

    FilmItem* film = static_cast<FilmItem*>( index.internalPointer() );
    film->ResetViewsCounter();
    dataChanged( index, index );
}


QModelIndex FilmsListModel::GetFilmIndex( QString title ) const
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


void FilmsListModel::SaveToFileSync( QString fileName )
{
    if( isDatabaseChanged && !isReadonly )
    {
        QMutexLocker locker( &mutexDataEdit );
        isDatabaseChanged = false;
        QString errorString;

        if( !FilmsListLoader::Save( rootItem, fileName, &errorString ) )
        {
            emit DatabaseWriteError( errorString );
        }
    }
}


void FilmsListModel::SaveToFileAsync( QString fileName )
{
    if( savingTimer.isActive() )
    {
        savingTimer.stop();
    }

    std::thread( &FilmsListModel::SaveToFileSync, this, fileName ).detach();
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

