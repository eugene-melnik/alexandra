/*************************************************************************************************
 *                                                                                                *
 *  file: searchwindow.cpp                                                                        *
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

#include "searchwindow.h"

#include <QMessageBox>
#include <QPushButton>
#include <QStringList>

SearchWindow::SearchWindow( const QList<Film>* f, QWidget* parent )
    : QDialog( parent ), films( f )
{
    setupUi( this );
    ConfigureTable();
    eKeywords->setFocus();

    connect( bSearch, &QPushButton::clicked, this, &SearchWindow::Search );
    connect( bOk, &QPushButton::clicked, this, &SearchWindow::OkButtonClicked );
    connect( twResult, &QTableWidget::itemDoubleClicked, this, &SearchWindow::OkButtonClicked );

    connect( bSelectAll, &QPushButton::clicked, this, &SearchWindow::SelectAll );
    connect( bUnselectAll, &QPushButton::clicked, this, &SearchWindow::UnselectAll );
}

void SearchWindow::Search()
{
    if( eKeywords->text().isEmpty() )
    {
        QMessageBox::information( this, tr( "Search" ), tr( "Nothing to search. Input keyword first." ) );
        return;
    }

    QStringList words = eKeywords->text().split( " " );

    foreach( const QString& word, words )
    {
        foreach( Film film, *films )
        {
            if( cTitle->isChecked() && film.GetTitle().contains( word, Qt::CaseInsensitive ) )
                founded.push_back( film );

            if( cTags->isChecked() && film.GetTags().contains( word, Qt::CaseInsensitive ) )
                founded.push_back( film );

            if( cGenre->isChecked() && film.GetGenre().contains( word, Qt::CaseInsensitive ) )
                founded.push_back( film );

            if( cStarring->isChecked() && film.GetStarring().contains( word, Qt::CaseInsensitive ) )
                founded.push_back( film );

            if( cDirector->isChecked() && film.GetDirector().contains( word, Qt::CaseInsensitive ) )
                founded.push_back( film );

            if( cProducer->isChecked() && film.GetProducer().contains( word, Qt::CaseInsensitive ) )
                founded.push_back( film );

            if( cCountry->isChecked() && film.GetCountry().contains( word, Qt::CaseInsensitive ) )
                founded.push_back( film );

            if( cDescription->isChecked() && film.GetDescription().contains( word, Qt::CaseInsensitive ) )
                founded.push_back( film );
        }

        founded.sort();
        founded.unique();
    }

    if( founded.empty() )
    {
        QMessageBox::information( this, tr( "Search" ), tr( "Nothing was found." ) );
    }

    UpdateTable( founded );
}

void SearchWindow::OkButtonClicked()
{
    if( twResult->rowCount() != 0 )
    {
        emit FilmSelected( twResult->item( twResult->currentRow(), TitleColumn )->text() );
    }

    close();
}

void SearchWindow::SetOptionsIsChecked( bool isChecked )
{
    cTitle->setChecked( isChecked );
    cTags->setChecked( isChecked );
    cGenre->setChecked( isChecked );
    cStarring->setChecked( isChecked );
    cDirector->setChecked( isChecked );
    cProducer->setChecked( isChecked );
    cCountry->setChecked( isChecked );
    cDescription->setChecked( isChecked );
}

void SearchWindow::ConfigureTable()
{
    QStringList colNames;
    colNames << tr( "Title" )
             << tr( "Genre" )
             << tr( "Year" )
             << tr( "Director" )
             << tr( "Producer" )
             << tr( "Country" )
             << tr( "Rating" );

    twResult->setColumnCount( colNames.size() );
    twResult->setHorizontalHeaderLabels( colNames );
}

void SearchWindow::UpdateTable( std::list<Film>& founded )
{
    // Clear old data
    twResult->clearContents();

    // Configure rows
    twResult->setRowCount( founded.size() );
    int row = 0;

    foreach( const Film f, founded )
    {
        // Title
        QTableWidgetItem* title = new QTableWidgetItem( f.GetTitle() );
        twResult->setItem( row, TitleColumn, title );
        twResult->setColumnWidth( TitleColumn, 130 );

        // Genre
        QTableWidgetItem* genre = new QTableWidgetItem( f.GetGenre() );
        twResult->setItem( row, GenreColumn, genre );
        twResult->setColumnWidth( GenreColumn, 100 );

        // Year
        QTableWidgetItem* year = new QTableWidgetItem( f.GetYearStr() );
        twResult->setItem( row, YearColumn, year );
        twResult->setColumnWidth( YearColumn, 45 );

        // Director
        QTableWidgetItem* director = new QTableWidgetItem( f.GetDirector() );
        twResult->setItem( row, DirectorColumn, director );
        twResult->setColumnWidth( DirectorColumn, 120 );

        // Producer
        QTableWidgetItem* producer = new QTableWidgetItem( f.GetProducer() );
        twResult->setItem( row, ProducerColumn, producer);
        twResult->setColumnWidth( ProducerColumn, 120 );

        // Country
        QTableWidgetItem* country = new QTableWidgetItem( f.GetCountry() );
        twResult->setItem( row, CountryColumn, country);
        twResult->setColumnWidth( CountryColumn, 80 );

        // Rating
        QTableWidgetItem* rating = new QTableWidgetItem( f.GetRatingStr() );
        twResult->setItem( row, RatingColumn, rating);

        row++;
    }

    lTotalFounded->setText( QString::number( founded.size() ) );

    if( !founded.empty() )
    {
        twResult->setCurrentCell( 0, 0);
        founded.clear();
    }
}
