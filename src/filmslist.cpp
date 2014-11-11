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
    setColumnWidth( 0, s->value( "FilmList/CW0", 20 ).toInt() );
    setColumnWidth( 1, s->value( "FilmList/CW1", 150 ).toInt() );
    setColumnWidth( 2, s->value( "FilmList/CW2", 50 ).toInt() );
    setColumnWidth( 3, s->value( "FilmList/CW3", 110 ).toInt() );
    setColumnWidth( 4, s->value( "FilmList/CW4", 110 ).toInt() );
}

void FilmsList::SaveSettings( QSettings* s ) const
{
    // Columns' width
    s->setValue( "FilmList/CW0", columnWidth(0) );
    s->setValue( "FilmList/CW1", columnWidth(1) );
    s->setValue( "FilmList/CW2", columnWidth(2) );
    s->setValue( "FilmList/CW3", columnWidth(3) );
    s->setValue( "FilmList/CW4", columnWidth(4) );
}

int FilmsList::GetNumberOfFilms() const
{
    return( films.size() );
}

const Film* FilmsList::GetFilmAt( int i ) const
{
    return( &films.at( i ) );
}

const Film* FilmsList::GetFilmByTitle( const QString& t ) const
{
    for( int i = 0; i < films.size(); i++ ) {
        if( films.at(i).GetTitle() == t ) {
            return( &films.at(i) );
        }
    }

    return( new Film() );
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
    for( int i = 0; i < films.size(); i++ ) {
        if( films[i].GetTitle() == f.GetTitle() ) {
            films[i].SetNewData( f );
            isDatabaseChanged = true;
            emit DatabaseChanged();
            return;
        }
    }

}

void FilmsList::SelectRandomFilm()
{
    int n;

    do
    {
        n = qrand() % rowCount();
    }
    while( n == currentRow() );

    if( rowCount() != 0 ) {
        itemClicked( item( n, 0 ) );
        setCurrentItem( item( n, 0 ) ); // FIXME: fix this shi~
    }
}

void FilmsList::ItemSelected( QTableWidgetItem* i )
{
    QString selectedFilmTitle = item( i->row(), 1 )->text();
    currentFilm = GetFilmByTitle( selectedFilmTitle );
    emit FilmSelected( currentFilm );
}

void FilmsList::UpdateFilmsTable()
{
    // Clear old data
    clear();

    // Configure columns
    QStringList colNames;
    colNames << tr( "+" )
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

        // Favourite
        QTableWidgetItem* favourite = new QTableWidgetItem( f.GetIsFavouriteSign() );
        setItem( row, 0, favourite );

        // Title
        QTableWidgetItem* title = new QTableWidgetItem( f.GetTitle() );
        setItem( row, 1, title );

        // Year
        QTableWidgetItem* year = new QTableWidgetItem( f.GetYearStr() );
        setItem( row, 2, year );

        // Genre
        QTableWidgetItem* genre = new QTableWidgetItem( f.GetGenre() );
        setItem( row, 3, genre );

        // Director
        QTableWidgetItem* director = new QTableWidgetItem( f.GetDirector() );
        setItem( row, 4, director );

        // Rating
        QTableWidgetItem* rating = new QTableWidgetItem( f.GetRatingStr() );
        setItem( row, 5, rating);
    }

    // Select first film
    if( rowCount() != 0 ) {
        itemClicked( item( 0, 0 ) );
        setCurrentItem( item( 0, 0 ) ); // FIXME: fix this shi~
    }
}
