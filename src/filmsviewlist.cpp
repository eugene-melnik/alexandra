/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewlist.cpp                                                                       *
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

#include "filmsviewlist.h"

#include <QHeaderView>

FilmsViewList::FilmsViewList( QWidget* parent ) : QTableWidget( parent )
{
    // Appearance
    setAlternatingRowColors( true );
    setContextMenuPolicy( Qt::CustomContextMenu );
    setEditTriggers( QAbstractItemView::NoEditTriggers );
    setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode( QAbstractItemView::SingleSelection );
    setShowGrid( false );
    setSortingEnabled( true );
    setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );

    verticalHeader()->setVisible( false );

    horizontalHeader()->setHighlightSections( false );
    horizontalHeader()->setStretchLastSection( true );
    horizontalHeader()->setDefaultSectionSize( 20 );
    horizontalHeader()->setMinimumSectionSize( 20 );

    // Signals
    connect( this, &FilmsViewList::itemSelectionChanged, this, &FilmsViewList::ItemClickedSlot );
    connect( this, &FilmsViewList::itemActivated, this, &FilmsViewList::ItemDoubleClickedSlot );
    connect( this, &FilmsViewList::itemDoubleClicked, this, &FilmsViewList::ItemDoubleClickedSlot );
    connect( this, &FilmsViewList::customContextMenuRequested, this, &FilmsViewList::ContextMenuRequestedSlot );

    // Columns setup
    setColumnCount( colNames.size() );

    for( int i = 0; i < columnCount(); i++ )
    {
        QTableWidgetItem* item = new QTableWidgetItem( colNames.at(i) );
        item->setToolTip( colTooltips.at(i) );
        setHorizontalHeaderItem( i, item );
    }
}

void FilmsViewList::LoadSettings( AlexandraSettings* s )
{
    setColumnWidth( ViewedColumn, s->GetColumnViewedWidth() );
    setColumnWidth( FavouriteColumn, s->GetColumnFavouriteWidth() );
    setColumnWidth( TitleColumn, s->GetColumnTitleWidth() );
    setColumnWidth( YearColumn, s->GetColumnYearWidth() );
    setColumnWidth( GenreColumn, s->GetColumnGenreWidth() );
    setColumnWidth( DirectorColumn, s->GetColumnDirectorWidth() );

    verticalHeader()->setDefaultSectionSize( s->GetListRowHeight() );
    setStyleSheet( QString( "font-size: %1px" ).arg( s->GetListFontSize() ) );
}

void FilmsViewList::ReloadSettings( AlexandraSettings *s )
{
    verticalHeader()->setDefaultSectionSize( s->GetListRowHeight() );
    setStyleSheet( QString( "font-size: %1px" ).arg( s->GetListFontSize() ) );
}

void FilmsViewList::SaveSettings( AlexandraSettings* s ) const
{
    s->SetColumnViewedWidth( columnWidth( ViewedColumn )  );
    s->SetColumnFavouriteWidth( columnWidth( FavouriteColumn ) );
    s->SetColumnTitleWidth( columnWidth( TitleColumn ) );
    s->SetColumnYearWidth( columnWidth( YearColumn ) );
    s->SetColumnGenreWidth( columnWidth( GenreColumn ) );
    s->SetColumnDirectorWidth( columnWidth( DirectorColumn ) );
}

int FilmsViewList::AddItem( const Film& film, QColor background )
{
    int currentRow = GetItemsCount();

    setRowCount( currentRow + 1 );
    SetItem( currentRow, film, background );

    return( currentRow );
}

void FilmsViewList::SetItem( int n, const Film& film, QColor background )
{
    // Viewed
    QTableWidgetItem* viewed = new QTableWidgetItem( film.GetIsViewedSign() );
    viewed->setTextAlignment( Qt::AlignCenter );
    setItem( n, ViewedColumn, viewed );

    // Favourite
    QTableWidgetItem* favourite = new QTableWidgetItem( film.GetIsFavouriteSign() );
    favourite->setTextAlignment( Qt::AlignCenter );
    setItem( n, FavouriteColumn, favourite );

    // Title
    QTableWidgetItem* title = new QTableWidgetItem( film.GetTitle() );
    setItem( n, TitleColumn, title );

    // Year
    QTableWidgetItem* year = new QTableWidgetItem( film.GetYearStr() );
    year->setTextAlignment( Qt::AlignCenter );
    setItem( n, YearColumn, year );

    // Genre
    QTableWidgetItem* genre = new QTableWidgetItem( film.GetGenre() );
    setItem( n, GenreColumn, genre );

    // Director
    QTableWidgetItem* director = new QTableWidgetItem( film.GetDirector() );
    setItem( n, DirectorColumn, director );

    // Rating
    QTableWidgetItem* rating = new QTableWidgetItem( film.GetRatingStr() );
    rating->setTextAlignment( Qt::AlignCenter );
    setItem( n, RatingColumn, rating );

    if( background.isValid() )
    {
        viewed->setBackgroundColor( background );
        favourite->setBackgroundColor( background );
        title->setBackgroundColor( background );
        year->setBackgroundColor( background );
        genre->setBackgroundColor( background );
        director->setBackgroundColor( background );
        rating->setBackgroundColor( background );
    }
}

void FilmsViewList::SetCurrentItemTo( const Film film )
{
    SetItem( GetCurrentItemIndex(), film );
}

void FilmsViewList::RemoveItem( int n )
{
    removeRow( n );
}

void FilmsViewList::RemoveCurrentItem()
{
    RemoveItem( GetCurrentItemIndex() );
}

void FilmsViewList::Clear()
{
    clearContents();
    setRowCount( 0 );
}

void FilmsViewList::SelectItem( Film film )
{
    SelectItem( film.GetTitle() );
}

void FilmsViewList::SelectItem( QString title )
{
    for( int row = 0; row < GetItemsCount(); row++ )
    {
        if( item( row, TitleColumn )->text() == title )
        {
            SetCurrentItemIndex( row );
            return;
        }
    }

    SetCurrentItemIndex( 0 );
}

void FilmsViewList::SelectRandomItem()
{
    if( rowCount() > 0 )
    {
        int n;

        do
        {
            n = qrand() % rowCount();
        }
        while( n == GetCurrentItemIndex() );

        SetCurrentItemIndex( n );
    }
}

int FilmsViewList::GetItemsCount() const
{
    return( rowCount() );
}

int FilmsViewList::GetCurrentItemIndex() const
{
    return( currentRow() );
}

void FilmsViewList::SetCurrentItemIndex( int i )
{
    if( rowCount() > 0 )
    {
        if( (i >= rowCount()) || (i < 0) )
        {
            i = 0;
        }

        setCurrentCell( i, 0 );
        itemClicked( item( i, 0 ) );
    }
}

void FilmsViewList::ItemClickedSlot()
{
    emit ItemClicked( this->item( currentRow(), TitleColumn )->text() );
}

void FilmsViewList::ItemDoubleClickedSlot()
{
    emit ItemDoubleClicked( this->item( currentRow(), TitleColumn )->text() );
}

void FilmsViewList::ContextMenuRequestedSlot( QPoint p )
{
    if( rowCount() > 0 )
    {
        SetCurrentItemIndex( GetCurrentItemIndex() );
        emit ContextMenuRequested( p );
    }
}
