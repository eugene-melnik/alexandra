/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewgrid.cpp                                                                       *
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

#include "filmsviewgrid.h"

#include <QBrush>
#include <QHeaderView>
#include <QPalette>

FilmsViewGrid::FilmsViewGrid( QWidget* parent ) : QListView( parent )
{
    // Appearance
    setContextMenuPolicy( Qt::CustomContextMenu );
    setResizeMode( QListView::Adjust );
    setViewMode( QListView::IconMode );
    setWrapping( true );

    setVerticalScrollMode( QAbstractItemView::ScrollPerPixel ); // WTF

    // Signals
    connect( this, &FilmsViewGrid::clicked, this, &FilmsViewGrid::ItemClickedSlot );
    connect( this, &FilmsViewGrid::activated, this, &FilmsViewGrid::ItemClickedSlot );
    connect( this, &FilmsViewGrid::doubleClicked, this, &FilmsViewGrid::ItemDoubleClickedSlot );
    connect( this, &FilmsViewGrid::customContextMenuRequested, this, &FilmsViewGrid::ContextMenuRequestedSlot );
}

void FilmsViewGrid::LoadSettings( AlexandraSettings* s )
{
    settings = s;
    setStyleSheet( QString( "font-size: %1pt" ).arg( s->GetGridFontSize() ) );

    model = new FilmViewGridModel( settings, this );
    setModel( model );
}

void FilmsViewGrid::ReloadSettings( AlexandraSettings* s )
{
    setStyleSheet( QString( "font-size: %1pt" ).arg( s->GetGridFontSize() ) );
}

int FilmsViewGrid::AddItem( const Film& film, QColor background )
{
    model->AppendItem( film, background );
    return( GetItemsCount() - 1 );
}

void FilmsViewGrid::SetItem( int n, const Film& film, QColor background )
{
    model->SetItem( n, film, background );
}

void FilmsViewGrid::SetCurrentItemTo( const Film film )
{
    SetItem( GetCurrentItemIndex(), film );
}

void FilmsViewGrid::RemoveItem( int n )
{
    model->RemoveRow( n );
    SetCurrentItemIndex( n );
}

void FilmsViewGrid::RemoveCurrentItem()
{
    RemoveItem( GetCurrentItemIndex() );
}

void FilmsViewGrid::Clear()
{
    model->Clear();
}

void FilmsViewGrid::SelectItem( Film film )
{
    SelectItem( film.GetTitle() );
}

void FilmsViewGrid::SelectItem( QString title )
{
    SetCurrentItemIndex( model->GetItemIndexByTitle( title ) );
}

void FilmsViewGrid::SelectRandomItem()
{
    if( GetItemsCount() > 0 )
    {
        int n;

        do
        {
            n = qrand() % model->rowCount();
        }
        while( n == GetCurrentItemIndex() );

        SetCurrentItemIndex( n );
    }
}

int FilmsViewGrid::GetItemsCount() const
{
    return( model->rowCount() );
}

int FilmsViewGrid::GetCurrentItemIndex() const
{
    return( currentIndex().row() );
}

void FilmsViewGrid::SetCurrentItemIndex( int i )
{
    if( GetItemsCount() > 0 )
    {
        if( i < 0 )
        {
            i = 0;
        }
        else if( i >= GetItemsCount() )
        {
            i = GetItemsCount() - 1;
        }

        setCurrentIndex( model->index( i, 0 ) );
        clicked( model->index( i, 0 ) );
    }
}

void FilmsViewGrid::ItemClickedSlot( QModelIndex i )
{
    emit ItemClicked( model->GetItemTitle( i.row() ) );
}

void FilmsViewGrid::ItemDoubleClickedSlot( QModelIndex i )
{
    emit ItemDoubleClicked( model->GetItemTitle( i.row() ) );
}

void FilmsViewGrid::ContextMenuRequestedSlot( QPoint p )
{
    if( GetItemsCount() > 0 )
    {
        SetCurrentItemIndex( GetCurrentItemIndex() );
        emit ContextMenuRequested( p );
    }
}
