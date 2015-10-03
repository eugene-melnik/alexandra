/*************************************************************************************************
 *                                                                                                *
 *  file: filmslist.cpp                                                                           *
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

#include "debug.h"
#include "filmslist.h"
#include "version.h"

#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <thread>

FilmsList::FilmsList( AlexandraSettings* s, QObject* parent )
    : QObject( parent ), settings( s ), films( new QList<Film>() )
{ }

FilmsList::~FilmsList()
{
    // Behavior on program exit while data loading
    // Wait for 5 seconds
    if( asyncSaveToFileMutex.tryLock( 5000 ) )
    {
        asyncSaveToFileMutex.unlock();
    }

    delete films;
}

void FilmsList::LoadFromFile( const QString& fileName )
{
    DebugPrintFuncA( "FilmsList::LoadFromFile", fileName );

    currentFilm = nullptr;
    films->clear();

    QFile file( fileName );

    if( !file.exists() )
    {
        emit DatabaseLoaded();
        emit DatabaseIsEmpty();
        return;
    }

    if( file.open( QIODevice::ReadOnly ) )
    {
        // Reading from file
        QDataStream stream( &file );
        QString databaseHeader;
        quint8 databaseVersion;

        stream >> databaseHeader;
        stream >> databaseVersion;

        if( databaseHeader == Alexandra::databaseHeader
                && databaseVersion == Alexandra::databaseVersion )
        {
            stream >> *films;
        }

        std::sort( films->begin(), films->end() );

        DebugPrintFuncDone( "FilmsList::LoadFromFile" );

        emit DatabaseLoaded();

        // Is Empty
        if( films->isEmpty() )
        {
            isDatabaseChanged = true;
            emit DatabaseIsEmpty();
        }

        // Is Readonly
        if( !QFileInfo( fileName ).isWritable() )
        {
            emit DatabaseIsReadonly();
        }
    }
    else // Is Read Error
    {
        emit DatabaseLoaded();
        emit DatabaseReadError();
        DebugPrint( "Failed!" );
    }

    file.close();
}

void FilmsList::SaveToFile( const QString& fileName )
{
    if( isDatabaseChanged )
    {
        DebugPrintFuncA( "FilmsList::SaveToFile", fileName );
        asyncSaveToFileMutex.lock();

        QFile file( fileName );

        if( !file.exists() )
        {
            QString databaseDir = QFileInfo( fileName ).absolutePath();

            if( !QDir().exists( databaseDir ) )
            {
                QDir().mkdir( databaseDir );
            }
        }

        if( file.open( QIODevice::WriteOnly ) )
        {
            QDataStream stream( &file );

            stream << Alexandra::databaseHeader;
            stream << Alexandra::databaseVersion;
            stream << *films;
        }
        else
        {
            emit DatabaseWriteError();
        }

        file.close();
        isDatabaseChanged = false;

        asyncSaveToFileMutex.unlock();
        DebugPrintFuncDone( "FilmsList::SaveToFile" );
    }
}

void FilmsList::SaveToFileAsync( const QString& fileName )
{
    std::thread( &FilmsList::SaveToFile, this, fileName ).detach();
}

int FilmsList::GetNumberOfFilms() const
{
    return( films->size() );
}

const Film *FilmsList::GetFilmByTitle( const QString& title )
{
    for( int i = 0; i < films->size(); i++ )
    {
        if( films->at(i).GetTitle() == title )
        {
            return( &(films->at(i)) );
        }
    }

    return( nullptr );
}

const QList<Film>* FilmsList::GetFilmsList() const
{
    return( films );
}

QStringList FilmsList::GetTitlesList() const
{
    QStringList res;

    for( const Film& film : *films )
    {
        res.append( film.GetTitle() );
    }

    return( res );
}

QStringList* FilmsList::GetFileNamesList() const
{
    QStringList* res = new QStringList();

    for( const Film& film : *films )
    {
        res->append( film.GetFileName() );
    }

    return( res );
}

QList<Film*>* FilmsList::GetUnavailablesList()
{
    DebugPrintFunc( "FilmsList::GetUnavailableFilms" );

    QList<Film*>* f = new QList<Film*>();

    for( QList<Film>::iterator iter = films->begin(); iter < films->end(); iter++ )
    {
        if( !QFileInfo( iter->GetFileName() ).exists() )
        {
            f->append( &(*iter) );
        }
    }

    return( f );
}

const Film* FilmsList::GetCurrentFilm() const
{
    return( currentFilm );
}

QString FilmsList::GetCurrentFilmTitle() const
{
    if( currentFilm != nullptr )
    {
        return( currentFilm->GetTitle() );
    }
    else
    {
        return( QString() );
    }
}

QString FilmsList::GetCurrentFilmFileName() const
{
    if( currentFilm != nullptr )
    {
        return( currentFilm->GetFileName() );
    }
    else
    {
        return( QString() );
    }
}

int FilmsList::GetFilmsCount() const
{
    return( films->size() );
}

int FilmsList::GetIsViewedCount() const
{
    int isViewedCount = 0;

    for( const Film& film : *films )
    {
        if( film.GetIsViewed() )
        {
            isViewedCount++;
        }
    }

    return( isViewedCount );
}

int FilmsList::GetIsFavouriteCount() const
{
    int isFavouriteCount = 0;

    for( const Film& film : *films )
    {
        if( film.GetIsFavourite() )
        {
            isFavouriteCount++;
        }
    }

    return( isFavouriteCount );
}

void FilmsList::AddFilm( const Film& film )
{
    DebugPrintFuncA( "FilmsList::AddFilm", film.GetFileName() );

    films->append( film );
    std::sort( films->begin(), films->end() );

    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::AddFilms( const QList<Film>* newFilms )
{
    DebugPrintFuncA( "FilmsList::AddFilms", newFilms->size() );

    *films += *newFilms;
    std::sort( films->begin(), films->end() );

    DebugPrintFuncDone( "FilmsList::AddFilms" );

    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::SetCurrentFilm( const QString& title )
{
    for( Film& film : *films )
    {
        if( film.GetTitle() == title )
        {
            currentFilm = &film;
            return;
        }
    }
}

void FilmsList::ChangeCurrentFilm( const Film& f )
{
    currentFilm->SetNewData( f );
    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::SetCurrentFilmIsViewed( bool b )
{
    currentFilm->SetIsViewed( b );
    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::SetCurrentFilmIsFavourite( bool b )
{
    currentFilm->SetIsFavourite( b );
    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::IncCurrentFilmViewsCounter()
{
    currentFilm->SetIsViewed( true );
    currentFilm->IncViewsCounter();
    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::RemoveCurrentFilm()
{
    RemoveFilm( *currentFilm );
    currentFilm = nullptr;
}

void FilmsList::RemoveFilmByTitle( const QString& title )
{
    DebugPrintFuncA( "FilmsList::RemoveFilmByTitle", title );

    for( const Film& film : *films )
    {
        if( film.GetTitle() == title )
        {
            RemoveFilm( film );
            break;
        }
    }

}

void FilmsList::FilmsMoved()
{
    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::EraseAll()
{
    DebugPrintFunc( "FilmsList::EraseAll" );
    QString postersDir = settings->GetPostersDirPath();

    for( const Film& film : *films )
    {
        if( film.GetIsPosterExists() == true )
        {
            QFile( postersDir + "/" + film.GetPosterName() ).remove();
        }
    }

    films->clear();

    DebugPrintFuncDone( "FilmsList::EraseAll" );

    currentFilm = nullptr;
    isDatabaseChanged = true;

    emit DatabaseLoaded();
    emit DatabaseChanged();
    emit DatabaseIsEmpty();
}

void FilmsList::ResetViews()
{
    DebugPrintFunc( "FilmsList::ResetViews" );

    for( Film& film : *films )
    {
        film.SetIsViewed( false );
        film.SetViewCounter( 0 );
    }

    DebugPrintFuncDone( "FilmsList::ResetViews" );

    isDatabaseChanged = true;

    emit DatabaseLoaded();
    emit DatabaseChanged();
}

void FilmsList::RemoveFilm( const Film& film )
{
    DebugPrintFuncA( "FilmsList::ResetViews", film.GetFileName() );

    // Remove poster image
    if( film.GetIsPosterExists() == true )
    {
        QString posterFileName = settings->GetPostersDirPath() + "/" + film.GetPosterName();
        QFile( posterFileName ).remove();
    }

    // Remove record from database
    films->removeOne( film );

    DebugPrintFuncDone( "FilmsList::RemoveFilm" );

    isDatabaseChanged = true;
    emit DatabaseChanged();

    if( films->isEmpty() )
    {
        emit DatabaseIsEmpty();
    }
}
