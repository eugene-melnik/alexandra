#include "filmslist.h"
#include "version.h"

#include <algorithm>
#include <QDataStream>
#include <QFile>

FilmsList::FilmsList( QWidget* parent ) : QTableWidget( parent )
{
    isDatabaseChanged = false;

    connect( this, SIGNAL( itemClicked(QTableWidgetItem*) ), this, SLOT( ItemSelected(QTableWidgetItem*) ) );   // mouse click
    connect( this, SIGNAL( itemActivated(QTableWidgetItem*) ), this, SLOT( ItemSelected(QTableWidgetItem*) ) ); // Enter key press
    connect( this, SIGNAL( DatabaseChanged() ), this, SLOT( UpdateFilmsTable() ) );
}

void FilmsList::LoadDatabase( QString databaseFileName )
{
    QFile dbFile( databaseFileName );
    films.clear();

    if( dbFile.open( QIODevice::ReadOnly ) )
    {
        QDataStream stream( &dbFile );
        stream >> films;
    }

    dbFile.close();
    emit DatabaseChanged();
}

void FilmsList::SaveDatabase( QString databaseFileName )
{
    if( isDatabaseChanged ) {
        QFile dbFile( databaseFileName );

        if( dbFile.open( QIODevice::WriteOnly ) )
        {
            QDataStream stream( &dbFile );
            stream << films;
        }

        dbFile.close();
        isDatabaseChanged = false;
    }
}

void FilmsList::LoadSettings( QSettings* s )
{
    // Columns' width
    setColumnWidth( ViewedColumn, s->value( "FilmList/CW0", 20 ).toInt() );
    setColumnWidth( FavouriteColumn, s->value( "FilmList/CW1", 20 ).toInt() );
    setColumnWidth( TitleColumn, s->value( "FilmList/CW2", 150 ).toInt() );
    setColumnWidth( YearColumn, s->value( "FilmList/CW3", 50 ).toInt() );
    setColumnWidth( GenreColumn, s->value( "FilmList/CW4", 110 ).toInt() );
    setColumnWidth( DirectorColumn, s->value( "FilmList/CW5", 110 ).toInt() );
}

void FilmsList::SaveSettings( QSettings* s ) const
{
    // Columns' width
    s->setValue( "FilmList/CW0", columnWidth( ViewedColumn )  );
    s->setValue( "FilmList/CW1", columnWidth( FavouriteColumn ) );
    s->setValue( "FilmList/CW2", columnWidth( TitleColumn ) );
    s->setValue( "FilmList/CW3", columnWidth( YearColumn ) );
    s->setValue( "FilmList/CW4", columnWidth( GenreColumn ) );
    s->setValue( "FilmList/CW5", columnWidth( DirectorColumn ) );
}

int FilmsList::GetNumberOfFilms() const
{
    return( films.size() );
}

const Film* FilmsList::GetFilmAt( int i ) const
{
    return( &films.at( i ) );
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
    films.removeOne( *currentFilm );
    isDatabaseChanged = true;

    emit DatabaseChanged();
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
}

void FilmsList::EditCurrentFilm( Film f )
{
    currentFilm->SetNewData( f );
    isDatabaseChanged = true;
    emit DatabaseChanged();
}

void FilmsList::SelectRandomFilm()
{
    int n;

    do {
        n = qrand() % rowCount();
    }
    while( n == currentRow() );

    SetCursorOnRow( n );
}

void FilmsList::SetCurrentIsViewed( bool b )
{
    currentFilm->SetIsViewed( b );
    setItem( currentRow(), ViewedColumn, new QTableWidgetItem( currentFilm->GetIsViewedSign() ) );
    isDatabaseChanged = true;
}

void FilmsList::SetCurrentIsFavourite( bool b )
{
    currentFilm->SetIsFavourite( b );
    setItem( currentRow(), FavouriteColumn, new QTableWidgetItem( currentFilm->GetIsFavouriteSign() ) );
    isDatabaseChanged = true;
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
    // Clear old data
    clear();

    // Configure columns
    QStringList colNames;
    colNames << tr( "V" )
             << tr( "F" )
             << tr( "Title" )
             << tr( "Year" )
             << tr( "Genre" )
             << tr( "Director" )
             << tr( "Rating" );
    setColumnCount( colNames.size() );
    setHorizontalHeaderLabels( colNames );

    // Configure rows
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
    }

    // Select first film
    SetCursorOnRow( 0 );
}

void FilmsList::SetCursorOnRow( int row )
{
    if( rowCount() != 0 ) {
        itemClicked( item( row, 0 ) );
        setCurrentItem( item( row, 0 ) ); // FIXME: fix this shi~
    }
}
