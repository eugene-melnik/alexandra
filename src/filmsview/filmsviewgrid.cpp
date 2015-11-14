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

#include "alexandrasettings.h"
#include "filmsviewgrid.h"

#include <QBrush>
#include <QFileInfo>
#include <QHeaderView>
#include <QScrollBar>

FilmsViewGrid::FilmsViewGrid( QWidget* parent ) : QListView( parent )
{
    // Appearance
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setContextMenuPolicy( Qt::CustomContextMenu );
    setResizeMode( QListView::Adjust );
    setViewMode( QListView::IconMode );
    setWrapping( true );
    setSpacing( 5 );

    //setVerticalScrollMode( QAbstractItemView::ScrollPerPixel ); // WTF: Qt bug

    // Signals
    connect( this, &FilmsViewGrid::clicked, this, &FilmsViewGrid::ItemClickedSlot );
    connect( this, &FilmsViewGrid::activated, this, &FilmsViewGrid::ItemClickedSlot );
    connect( this, &FilmsViewGrid::doubleClicked, this, &FilmsViewGrid::ItemDoubleClickedSlot );
    connect( this, &FilmsViewGrid::customContextMenuRequested, this, &FilmsViewGrid::ContextMenuRequestedSlot );
}

void FilmsViewGrid::LoadSettings()
{
    AlexandraSettings* settings  = AlexandraSettings::GetInstance();

    setStyleSheet( QString( "font-size: %1pt" ).arg( settings->GetGridFontSize() ) );

    model = new FilmViewGridModel( this );
    setModel( model );
}

void FilmsViewGrid::ReloadSettings()
{
    setStyleSheet( QString( "font-size: %1pt" ).arg( AlexandraSettings::GetInstance()->GetGridFontSize() ) );
}

int FilmsViewGrid::AddItem( const Film& film )
{
    model->AppendItem( film );
    return( GetItemsCount() - 1 );
}

void FilmsViewGrid::RemoveItem( int n )
{
    model->RemoveRow( n );
    SetCurrentItemIndex( n );
}

void FilmsViewGrid::RemoveItemByTitle( const QString& title )
{
    for( int i = 0; i < GetItemsCount(); i++ )
    {
        QModelIndex index = model->index( i, 0 );
        QString t = model->data( index, Qt::DisplayRole ).toString();

        if( t == title )
        {
            model->RemoveRow( i );
            return;
        }
    }
}

void FilmsViewGrid::SelectRandomItem()
{
    if( GetItemsCount() > 1 )
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

QStringList FilmsViewGrid::GetSelectedItemsList() const
{
    QStringList res;

    for( const auto& index : selectedIndexes() )
    {
        res.append( model->data( index, Qt::DisplayRole ).toString() );
    }

    return( res );
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

void FilmsViewGrid::keyPressEvent(QKeyEvent *event)
{
    QListView::keyPressEvent( event );

    switch( event->key() )
    {
        case Qt::Key_Home :
        {
            SetCurrentItemIndex( 0 );
            ItemClickedSlot( currentIndex() );
            break;
        }
        case Qt::Key_End :
        {
            SetCurrentItemIndex( GetItemsCount() - 1 );
            ItemClickedSlot( currentIndex() );
            break;
        }
        case Qt::Key_Down :
        case Qt::Key_Up :
        case Qt::Key_Left :
        case Qt::Key_Right :
        case Qt::Key_PageUp :
        case Qt::Key_PageDown :
        {
            ItemClickedSlot( currentIndex() );
        }
    }
}

void FilmsViewGrid::updateGeometries()
{
    QListView::updateGeometries();
    verticalScrollBar()->setSingleStep( AlexandraSettings::GetInstance()->GetGridItemSize() / 5 );
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
