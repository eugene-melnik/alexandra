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

#include "filmslist.h"
#include "version.h"

#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QtConcurrentRun>

FilmsList::FilmsList( AlexandraSettings* s, QObject* parent ) : QObject( parent )
{
    settings = s;
    films = new QList<Film>();
}

FilmsList::~FilmsList()
{
    delete films;
}

void FilmsList::LoadFromFile( const QString& fileName )
{
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
                && databaseVersion == Alexandra::databaseVersion ) // need to optimize
        {
            stream >> *films;
        }

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
    }

    file.close();
}

void FilmsList::SaveToFile( const QString& fileName )
{
    if( isDatabaseChanged )
    {
        asyncSaveToFileMutex.lock();

        std::sort( films->begin(), films->end() );

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
    }
}

void FilmsList::SaveToFileAsync( const QString& fileName )
{
    QtConcurrent::run( this, &FilmsList::SaveToFile, fileName );
}

int FilmsList::GetNumberOfFilms() const
{
    return( films->size() );
}

const Film* FilmsList::GetFilmAt( int i ) const
{
    return( &(films->at(i)) );
}

const Film *FilmsList::GetFilmByTitle( QString title )
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

QList<Film>* FilmsList::GetFilmsFilteredBy( const QString& key ) const
{
    QList<Film>* f = new QList<Film>();

    for( QList<Film>::iterator i = films->begin(); i < films->end(); i++ )
    {
        if( i->GetTitle().contains( key, Qt::CaseInsensitive ) )
        {
            f->append( *i );
        }
    }

    return( f );
}

QStringList* FilmsList::GetFilmsFileNames() const
{
    QStringList* res = new QStringList();

    for( QList<Film>::iterator i = films->begin(); i < films->end(); i++ )
    {
        *res << i->GetFileName();
    }

    return( res );
}

QList<Film*>* FilmsList::GetUnavailableFilms()
{
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
        return( "" );
    }
}

QString FilmsList::GetCurrentFilmFileName() const
{
    return( currentFilm->GetFileName() );
}

int FilmsList::GetFilmsCount() const
{
    return( films->size() );
}

int FilmsList::GetIsViewedCount() const
{
    int isViewedCount = 0;

    for( QList<Film>::iterator f = films->begin(); f < films->end(); f++ )
    {
        if( f->GetIsViewed() )
        {
            isViewedCount++;
        }
    }

    return( isViewedCount );
}

int FilmsList::GetIsFavouriteCount() const
{
    int isFavouriteCount = 0;

    for( QList<Film>::iterator f = films->begin(); f < films->end(); f++ )
    {
        if( f->GetIsFavourite() )
        {
            isFavouriteCount++;
        }
    }

    return( isFavouriteCount );
}

void FilmsList::AddFilm( Film film )
{
    films->append( film );
    std::sort( films->begin(), films->end() );

    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::AddFilms( const QList<Film>* newFilms )
{
    *films += *newFilms;

    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::SetCurrentFilm( const QString& title )
{
    for( QList<Film>::iterator f = films->begin(); f < films->end(); f++ )
    {
        if( f->GetTitle() == title )
        {
            currentFilm = &(*f);
            return;
        }
    }
}

void FilmsList::ChangeCurrentFilm( Film f )
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
    currentFilm->IncViewsCounter();
    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::RemoveCurrentFilm()
{
    // Remove poster image
    if( currentFilm->GetIsPosterExists() == true )
    {
        QString posterFileName = settings->GetPostersDirPath() + "/" + currentFilm->GetPosterName();
        QFile( posterFileName ).remove();
    }

    // Remove record from database
    films->removeOne( *currentFilm );
    isDatabaseChanged = true;
    currentFilm = nullptr;
    emit DatabaseChanged();

    if( films->isEmpty() )
    {
        emit DatabaseIsEmpty();
    }
}

void FilmsList::FilmsMoved()
{
    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::EraseAll()
{
    QString postersDir = settings->GetPostersDirPath();

    for( QList<Film>::iterator i = films->begin(); i < films->end(); i++ )
    {
        if( i->GetIsPosterExists() == true )
        {
            QFile( postersDir + "/" + i->GetPosterName() ).remove();
        }
    }

    films->clear();

    currentFilm = nullptr;
    isDatabaseChanged = true;

    emit DatabaseLoaded();
    emit DatabaseChanged();
    emit DatabaseIsEmpty();
}

void FilmsList::ResetViews()
{
    for( QList<Film>::iterator i = films->begin(); i < films->end(); i++ )
    {
        i->SetIsViewed( false );
        i->SetViewCounter( 0 );
    }

    isDatabaseChanged = true;

    emit DatabaseLoaded();
    emit DatabaseChanged();
}
