#include "filmslist.h"
#include "version.h"

#include <QDataStream>
#include <QFile>

FilmsList::FilmsList( QWidget* parent ) : QTableWidget( parent )
{
    connect( this, SIGNAL( DatabaseChanged() ), this, SLOT( UpdateFilmsTable() ) );
}

void FilmsList::LoadDatabase( QString dataDirectory )
{
    dataDirectory.append( "database.dat" );
    QFile dbFile( dataDirectory );

    if( dbFile.open( QIODevice::ReadOnly ) )
    {
        QDataStream stream( &dbFile );
        stream >> films;
    }

    dbFile.close();
    emit DatabaseChanged();
}

void FilmsList::SaveDatabase( QString dataDirectory )
{
    dataDirectory.append( "database.dat" );
    QFile dbFile( dataDirectory );

    if( dbFile.open( QIODevice::WriteOnly ) )
    {
        QDataStream stream( &dbFile );
        stream << films;
    }

    dbFile.close();
}

void FilmsList::LoadSettings( const QSettings& s )
{
    // Columns' width
    setColumnWidth( 0, s.value( "FilmList/CW0", 20 ).toInt() );
    setColumnWidth( 1, s.value( "FilmList/CW1", 150 ).toInt() );
    setColumnWidth( 2, s.value( "FilmList/CW2", 50 ).toInt() );
    setColumnWidth( 3, s.value( "FilmList/CW3", 110 ).toInt() );
    setColumnWidth( 4, s.value( "FilmList/CW4", 110 ).toInt() );
    setColumnWidth( 5, s.value( "FilmList/CW5", 50 ).toInt() );
}

void FilmsList::SaveSettings( QSettings& s ) const
{
    // Columns' width
    s.setValue( "FilmList/CW0", columnWidth(0) );
    s.setValue( "FilmList/CW1", columnWidth(1) );
    s.setValue( "FilmList/CW2", columnWidth(2) );
    s.setValue( "FilmList/CW3", columnWidth(3) );
    s.setValue( "FilmList/CW4", columnWidth(4) );
    s.setValue( "FilmList/CW5", columnWidth(5) );
}

void FilmsList::AppendFilm( Film f )
{
    films.append( f );
    emit DatabaseChanged();
}

const Film& FilmsList::GetFilmAt( int i ) const
{
    return( films.at( i ) );
}

int FilmsList::GetNumberOfFilms() const
{
    return( films.size() );
}

const Film& FilmsList::GetFilmByTitle( const QString& t ) const
{
    for( int i = 0; i < films.size(); i++ )
    {
        if( films.at(i).title == t )
        {
            return( films.at(i) );
        }
    }

    return( *new Film() );
}

int FilmsList::GetIsViewedCount() const
{
    int res = 0;

    foreach (Film f, films)
    {
        if( f.isViewed ) res++;
    }

    return( res );
}

int FilmsList::GetIsFavouriteCount() const
{
    int res = 0;

    foreach (Film f, films)
    {
        if( f.isFavourite ) res++;
    }

    return( res );
}

void FilmsList::UpdateFilmsTable()
{
    // Clear old data
    clear();

    // Configure columns
    QStringList colNames;
    colNames.append( tr( "+" ) );
    colNames.append( tr( "Title" ) );
    colNames.append( tr( "Year" ) );
    colNames.append( tr( "Genre" ) );
    colNames.append( tr( "Director" ) );
    colNames.append( tr( "Rating" ) );
    setColumnCount( colNames.size() );
    setHorizontalHeaderLabels( colNames );

    // Configure rows
    setRowCount( this->GetNumberOfFilms() );

    for( int row = 0; row != this->rowCount(); row++ )
    {
        // Favourite
        QTableWidgetItem* favourite = new QTableWidgetItem( films.at(row).isFavourite ? "+" : "" );
        this->setItem( row, 0, favourite );

        // Title
        QTableWidgetItem* title = new QTableWidgetItem( films.at(row).title );
        this->setItem( row, 1, title );

        // Year
        QTableWidgetItem* year = new QTableWidgetItem( QString("%1").arg(films.at(row).year) );
        this->setItem( row, 2, year );

        // Genre
        QTableWidgetItem* genre = new QTableWidgetItem( films.at(row).genre );
        this->setItem( row, 3, genre );

        // Director
        QTableWidgetItem* director = new QTableWidgetItem( films.at(row).director );
        this->setItem( row, 4, director );

        // Rating
        QTableWidgetItem* rating = new QTableWidgetItem( QString("%1/10").arg(films.at(row).rating) );
        setItem( row, 5, rating);

        itemClicked( rating ); // dummy
    }
}
