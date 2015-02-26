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

#include <algorithm>
#include <QtConcurrentRun>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QStringList>
#include <QTableWidgetItem>

FilmsList::FilmsList( QWidget* parent ) : QTableWidget( parent )
{
    isDatabaseChanged = false;

    connect( this, SIGNAL( itemClicked(QTableWidgetItem*) ), this, SLOT( ItemSelected(QTableWidgetItem*) ) );   // mouse click
    connect( this, SIGNAL( itemActivated(QTableWidgetItem*) ), this, SLOT( ItemSelected(QTableWidgetItem*) ) ); // Enter key press
    connect( this, SIGNAL( DatabaseChanged() ), this, SLOT( UpdateFilmsTable() ) );
}

void FilmsList::LoadDatabase()
{
    films.clear();
    currentFilm = nullptr;
    QFile dbFile( settings->GetApplicationDatabaseFile() );

    if( !dbFile.exists() ) {
        emit DatabaseChanged();
        emit DatabaseIsEmpty();
        return;
    }

    if( dbFile.open( QIODevice::ReadOnly ) )
    {
        // Reading from file
        QDataStream stream( &dbFile );
        stream >> films;

        emit DatabaseChanged();

        if( films.isEmpty() )  // Database is empty
        {
            isDatabaseChanged = true;
            emit DatabaseIsEmpty();
        }

        if( !QFileInfo( settings->GetApplicationDatabaseFile() ).isWritable() )   // Database is readonly
        {
            emit DatabaseIsReadonly();
        }
    }
    else // Database is unavailable for reading
    {
        currentFilm = nullptr;
        emit DatabaseChanged();
        emit DatabaseReadError();
    }

    dbFile.close();
}

void FilmsList::SaveDatabase()
{
    if( isDatabaseChanged )
    {
        QtConcurrent::run( this, &FilmsList::SaveDatabaseConcurrent, settings->GetApplicationDatabaseFile() );
    }
}

void FilmsList::LoadSettings( AlexandraSettings* s )
{
    settings = s;

    // Columns' width
    setColumnWidth( ViewedColumn, s->GetFilmsListColumnViewedW() );
    setColumnWidth( FavouriteColumn, s->GetFilmsListColumnFavouriteW() );
    setColumnWidth( TitleColumn, s->GetFilmsListColumnTitleW() );
    setColumnWidth( YearColumn, s->GetFilmsListColumnYearW() );
    setColumnWidth( GenreColumn, s->GetFilmsListColumnGenreW() );
    setColumnWidth( DirectorColumn, s->GetFilmsListColumnDirectorW() );
}

void FilmsList::SaveSettings() const
{
    // Columns' width
    settings->SetFilmsListColumnViewedW( columnWidth( ViewedColumn )  );
    settings->SetFilmsListColumnFavouriteW( columnWidth( FavouriteColumn ) );
    settings->SetFilmsListColumnTitleW( columnWidth( TitleColumn ) );
    settings->SetFilmsListColumnYearW( columnWidth( YearColumn ) );
    settings->SetFilmsListColumnGenreW( columnWidth( GenreColumn ) );
    settings->SetFilmsListColumnDirectorW( columnWidth( DirectorColumn ) );

    // Current row
    if( currentFilm != nullptr ) {
        settings->SetFilmsListCurrentFilm( currentFilm->GetTitle() );
    }
}

int FilmsList::GetNumberOfFilms() const
{
    return( films.size() );
}

const Film* FilmsList::GetFilmAt( int i ) const
{
    return( &films.at( i ) );
}

const QList<Film>* FilmsList::GetFilmsList() const
{
    return( &films );
}

const Film* FilmsList::GetCurrentFilm() const
{
    return( currentFilm );
}

const QString& FilmsList::GetCurrentFilmTitle() const
{
    return( currentFilm->GetTitle() );
}

const QString& FilmsList::GetCurrentFilmFileName() const
{
    return( currentFilm->GetFileName() );
}

void FilmsList::RemoveCurrentFilm()
{
    // Remove poster image
    QString posterFileName = settings->GetFilmsListPostersDir() + "/" + currentFilm->GetPosterName();
    QFile( posterFileName ).remove();

    // Remove record from database
    films.removeOne( *currentFilm );
    isDatabaseChanged = true;
    currentFilm = nullptr;
    emit DatabaseChanged();

    if( films.isEmpty() ) {
        emit DatabaseIsEmpty();
    }
}

int FilmsList::GetIsViewedCount() const
{
    int res = 0;

    foreach( Film f, films ) {
        if( f.GetIsViewed() ) {
            res++;
        }
    }

    return( res );
}

int FilmsList::GetIsFavouriteCount() const
{
    int res = 0;

    foreach( Film f, films ) {
        if( f.GetIsFavourite() ) {
            res++;
        }
    }

    return( res );
}

void FilmsList::AppendFilm( Film f )
{
    films.append( f );
    std::sort( films.begin(), films.end() );
    isDatabaseChanged = true;

    emit DatabaseChanged();
    SetCursorOnFilm( f.GetTitle() );
}

void FilmsList::EditCurrentFilm( Film f )
{
    currentFilm->SetNewData( f );
    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::SelectRandomFilm()
{
    if( films.size() > 1 )
    {
        int n;

        do {
            n = qrand() % rowCount();
        }
        while( n == currentRow() );

        SetCursorOnRow( n );
    }
}

void FilmsList::SetCurrentIsViewed( bool b )
{
    currentFilm->SetIsViewed( b );
    setItem( currentRow(), ViewedColumn, new QTableWidgetItem( currentFilm->GetIsViewedSign() ) );
    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::SetCurrentIsFavourite( bool b )
{
    currentFilm->SetIsFavourite( b );
    setItem( currentRow(), FavouriteColumn, new QTableWidgetItem( currentFilm->GetIsFavouriteSign() ) );
    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::SelectFilm( const QString& title )
{
    SetCursorOnFilm( title );
}

void FilmsList::ItemSelected( QTableWidgetItem* i )
{
    QString selectedFilmTitle = item( i->row(), TitleColumn )->text();

    for( QList<Film>::iterator f = films.begin(); f < films.end(); f++ ) {
        if( f->GetTitle() == selectedFilmTitle ) {
            currentFilm = &(*f);
            emit FilmSelected( currentFilm );
        }
    }
}

void FilmsList::UpdateFilmsTable()
{
    // Saving selected row
    int selectedRow = currentRow();

    // Clear old data
    clear();

    // Configure columns
    QStringList colNames;
    colNames << tr( "V" )  // TODO: tooltips
             << tr( "F" )  //
             << tr( "Title" )
             << tr( "Year" )
             << tr( "Genre" )
             << tr( "Director" )
             << tr( "Rating" );
    setColumnCount( colNames.size() );
    setHorizontalHeaderLabels( colNames );

    // Configure rows
    bool highlightUnavailable = settings->GetFilmsListCheckFilesOnStartup();
    QColor unavailableColor = settings->GetFilmsListUnavailableFileColor();

    setRowCount( this->GetNumberOfFilms() );

    for( int row = 0; row < rowCount(); row++ )
    {
        const Film& f = films.at( row );

        // Viewed
        QTableWidgetItem* viewed = new QTableWidgetItem( f.GetIsViewedSign() );
        setItem( row, ViewedColumn, viewed );

        // Favourite
        QTableWidgetItem* favourite = new QTableWidgetItem( f.GetIsFavouriteSign() );
        setItem( row, FavouriteColumn, favourite );

        // Title
        QTableWidgetItem* title = new QTableWidgetItem( f.GetTitle() );
        setItem( row, TitleColumn, title );

        // Year
        QTableWidgetItem* year = new QTableWidgetItem( f.GetYearStr() );
        setItem( row, YearColumn, year );

        // Genre
        QTableWidgetItem* genre = new QTableWidgetItem( f.GetGenre() );
        setItem( row, GenreColumn, genre );

        // Director
        QTableWidgetItem* director = new QTableWidgetItem( f.GetDirector() );
        setItem( row, DirectorColumn, director );

        // Rating
        QTableWidgetItem* rating = new QTableWidgetItem( f.GetRatingStr() );
        setItem( row, RatingColumn, rating);

        // Highlighting
        if( highlightUnavailable && !QFileInfo( f.GetFileName() ).exists() ) {
            viewed->setBackgroundColor( unavailableColor );
            favourite->setBackgroundColor( unavailableColor );
            title->setBackgroundColor( unavailableColor );
            year->setBackgroundColor( unavailableColor );
            genre->setBackgroundColor( unavailableColor );
            director->setBackgroundColor( unavailableColor );
            rating->setBackgroundColor( unavailableColor );
        }
    }

    // Restore selection
    if( selectedRow == -1 ) {
        SetCursorOnFilm( settings->GetFilmsListCurrentFilm() );
    } else {
        SetCursorOnRow( selectedRow );
    }
}

void FilmsList::FilterBy( QString s )
{
    UpdateFilmsTable();

    if( !s.isEmpty() )
    {
        if( (rowCount() != 0) && !s.isEmpty() ) {
            for( int row = 0; row < rowCount(); row++ ) {
                if( !item( row, TitleColumn )->text().contains( s, Qt::CaseInsensitive ) ) {
                    removeRow( row-- );
                }
            }
        }

        SetCursorOnRow( 0 );
    }
}

void FilmsList::EraseDatabase()
{
    if( films.isEmpty() ) {
        QMessageBox::information( this,
                                  tr( "Erase database" ),
                                  tr( "Nothing to erase." ) );
    } else {
        QString postersDir = settings->GetFilmsListPostersDir();

        foreach( Film f, films ) {
            if( f.GetIsPosterExists() == true ) {
                QFile( postersDir + "/" + f.GetPosterName() ).remove();
            }
        }

        films.clear();

        isDatabaseChanged = true;
        emit DatabaseChanged();

        QMessageBox::information( this,
                                  tr( "Erase database" ),
                                  tr( "Done!" ) );
    }
}

void FilmsList::SetCursorOnRow( int row )
{
    if( ( rowCount() != 0 ) )
    {
        if( row >= rowCount() || ( row < 0 ) ) {
            row = 0;
        }

        setCurrentCell( row, 0 );
        itemClicked( item( row, 0 ) );
    }
}

void FilmsList::SetCursorOnFilm( const QString& title )
{
    for( int row = 0; row < rowCount(); row++ ) {
        if( item( row, TitleColumn )->text() == title ) {
            SetCursorOnRow( row );
            return;
        }
    }

    SetCursorOnRow( 0 );
}

void FilmsList::SaveDatabaseConcurrent( QString databaseFileName )
{
    QFile dbFile( databaseFileName );

    if( !dbFile.exists() ) {
        QString databaseDir = QFileInfo( databaseFileName ).absolutePath();

        if( !QDir().exists( databaseDir ) ) {
            QDir().mkdir( databaseDir );
        }
    }

    if( dbFile.open( QIODevice::WriteOnly ) )
    {
        QDataStream stream( &dbFile );
        stream << films;
    }

    dbFile.close();
    isDatabaseChanged = false;
}
