/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewlist.cpp                                                                       *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2016 Eugene Melnik <jeka7js@gmail.com>                                     *
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
#include "alexandrasettings.h"
#include "filmslist/filmslistmodel.h"
#include "tools/debug.h"

#include <QHeaderView>
#include <QMenu>


FilmsViewList::FilmsViewList( QWidget* parent ) : QTableView( parent )
{
      // Appearance
    setAlternatingRowColors( true );
    setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
    setEditTriggers( QAbstractItemView::NoEditTriggers );
    setContextMenuPolicy( Qt::CustomContextMenu );
    setSortingEnabled( true );
    setShowGrid( false );

    verticalHeader()->setVisible( false );
    horizontalHeader()->setSectionsMovable( true );
    horizontalHeader()->setHighlightSections( false );
    horizontalHeader()->setStretchLastSection( true );
    horizontalHeader()->setDefaultSectionSize( 50 );
    horizontalHeader()->setMinimumSectionSize( 20 );
    horizontalHeader()->setContextMenuPolicy( Qt::CustomContextMenu );

      // Signals
    connect( horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &FilmsViewList::ShowHeaderContextMenu );

    connect( this, &QTableView::customContextMenuRequested, this, [this] (const QPoint& pos)
    {
        if( currentIndex().isValid() )
        {
            emit ContextMenuRequested( pos, selectionModel()->currentIndex() );
        }
    });
}


void FilmsViewList::setModel( QAbstractItemModel* model )
{
    QTableView::setModel( model );

    connect( selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SIGNAL(CurrentChanged(QModelIndex)) );

    connect( model, &QAbstractItemModel::dataChanged, this, [this]
    {
        QModelIndex index = currentIndex();
        selectionModel()->currentChanged( index, index );
    } );
}


void FilmsViewList::LoadSettings()
{
    AlexandraSettings* settings = AlexandraSettings::GetInstance();
    QByteArray headerState = settings->GetListViewHeaderState();

    if( headerState.isEmpty() ) // First launch (or without settings file)
    {
        SetDefaultColumnsView();
    }
    else
    {
        horizontalHeader()->restoreState( headerState );
    }

    verticalHeader()->setDefaultSectionSize( settings->GetListRowHeight() );
    setStyleSheet( QString( "font-size: %1pt" ).arg( settings->GetListFontSize() ) );
}


void FilmsViewList::ReloadSettings()
{
    AlexandraSettings* settings = AlexandraSettings::GetInstance();
    verticalHeader()->setDefaultSectionSize( settings->GetListRowHeight() );
    setStyleSheet( QString( "font-size: %1pt" ).arg( settings->GetListFontSize() ) );
}


void FilmsViewList::SaveSettings() const
{
    AlexandraSettings::GetInstance()->SetListViewHeaderState( horizontalHeader()->saveState() );
}


QModelIndexList FilmsViewList::GetSelectedItemsList()
{
    QModelIndexList indexes = selectedIndexes();
    QMap<int,QModelIndex> resultIndexes;

    for( const QModelIndex& index : indexes )
    {
        int row = index.row();

        if( !resultIndexes.contains(row) )
        {
            resultIndexes.insert( row, index );
        }
    }

    return( resultIndexes.values() );
}


void FilmsViewList::keyPressEvent( QKeyEvent* event )
{
    switch( event->key() )
    {
        case Qt::Key_Home :
        {
            setCurrentIndex( model()->index( 0, 0 ) );
            break;
        }
        case Qt::Key_End :
        {
            setCurrentIndex( model()->index( model()->rowCount() - 1, 0 ) );
            break;
        }
        default :
        {
            QTableView::keyPressEvent( event );
        }
    }
}


void FilmsViewList::ShowHeaderContextMenu( const QPoint& pos )
{
      // Setup
    QHeaderView* header = horizontalHeader();
    QMenu menu( tr( "Columns" ) );

      // "Title" always enabled
      // "Filename" and "Poster" always disabled
    for( int column = 1; column < model()->columnCount() - 2; ++column )
    {
        QAction* action = menu.addAction( model()->headerData( column, Qt::Horizontal ).toString() );
        action->setCheckable( true );
        action->setChecked( !header->isSectionHidden( column ) );
        action->setData( column );
    }

    menu.addSeparator();
    QAction* resetAction = menu.addAction( tr( "Reset to defaults" ) );
    resetAction->setData( model()->columnCount() );

      // Show
    QAction* triggered = menu.exec( header->viewport()->mapToGlobal(pos) );

      // Processing
    if( triggered != nullptr )
    {
        int i = triggered->data().toInt();

        if( i < model()->columnCount() )
        {
            header->setSectionHidden( i, !header->isSectionHidden( i ) );
        }
        else // "Reset to defaults" item
        {
            RestoreColumnsOrder();
            SetDefaultColumnsView();
        }
    }
}


void FilmsViewList::SetDefaultColumnsView()
{
    DebugPrintFunc( "FilmsViewList::SetDefaultColumnsView" );

      // Move to start
    QHeaderView* header = horizontalHeader();
    header->moveSection( FilmItem::IsFavouriteColumn, 0 );
    header->moveSection( FilmItem::IsViewedColumn + 1, 0 );

      // Hide
    QList<FilmItem::Column> hiddenColumns =
    {
        FilmItem::OriginalTitleColumn, FilmItem::CountryColumn,  FilmItem::ProducerColumn,
        FilmItem::ScreenwriterColumn,  FilmItem::ComposerColumn, FilmItem::BudgetColumn,
        FilmItem::ViewsCountColumn,    FilmItem::TaglineColumn,  FilmItem::StarringColumn,
        FilmItem::DescriptionColumn,   FilmItem::TagsColumn,     FilmItem::FileNameColumn,
        FilmItem::PosterColumn
    };

    for( FilmItem::Column col : hiddenColumns )
    {
        header->hideSection( col );
    }

      // Show and resize
    QList< QPair<FilmItem::Column,int> > shownColumns =
    {
        { FilmItem::TitleColumn, 150 }, { FilmItem::YearColumn, 50 },
        { FilmItem::GenreColumn, 110 }, { FilmItem::DirectorColumn, 110 },
        { FilmItem::RatingColumn, 50 }, { FilmItem::IsViewedColumn, 20 },
        { FilmItem::IsFavouriteColumn, 20 }
    };

    for( auto& p : shownColumns )
    {
        header->showSection( p.first );
        header->resizeSection( p.first, p.second );
    }

    DebugPrintFuncDone( "FilmsViewList::SetDefaultColumnsView" );
}


void FilmsViewList::RestoreColumnsOrder()
{
    DebugPrintFunc( "FilmsViewList::RestoreColumnsOrder" );
    QHeaderView* header = horizontalHeader();

    for( int i = 0; i < model()->columnCount(); ++i )
    {
        header->moveSection( header->visualIndex(i), i );
    }

    DebugPrintFuncDone( "FilmsViewList::RestoreColumnsOrder" );
}

