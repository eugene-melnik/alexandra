/*************************************************************************************************
 *                                                                                                *
 *  file: searchedit.cpp                                                                          *
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
#include "searchedit.h"
#include "filmslist/filmitem.h"

#include <QCompleter>


SearchEdit::SearchEdit( QWidget* parent )
    : QLineEdit( parent ),
      menuSelectColumns( new SearchEditMenu( this ) )
{
      // Settings icon
    actionSettings = addAction( QIcon( ":/tool/spanner" ), QLineEdit::TrailingPosition );
    actionSettings->setToolTip( tr( "Select columns for filtration" ) );
    connect( actionSettings, &QAction::triggered, this, &SearchEdit::ShowMenu );

    connect( this, &SearchEdit::textChanged, this, [this] (const QString& s) { emit TextChanged( s, selectedColumns ); } );
}


void SearchEdit::LoadSettings()
{
    selectedColumns = AlexandraSettings::GetInstance()->GetQuickSearchFilter();

    for( int i = 0; i < actionsColumns.size(); ++i )
    {
        int currentColumn = actionsColumns[i]->data().toInt();

        if( selectedColumns.contains( currentColumn )  )
        {
            actionsColumns[i]->setChecked( true );
        }
    }
}


void SearchEdit::SaveSettings() const
{
    AlexandraSettings::GetInstance()->SetQuickSearchFilter( selectedColumns );
}


void SearchEdit::SetModel(QAbstractItemModel *model)
{
    sourceModel = model;

    QCompleter* completer = new QCompleter( model, this ); ///
    completer->setCaseSensitivity( Qt::CaseInsensitive );
    completer->setCompletionColumn( 0 );
    setCompleter( completer );

    SetupMenu();
}


void SearchEdit::SetupMenu()
{
    menuSelectColumns->clear();
    actionsColumns.clear();

      // Header
    QAction* a = menuSelectColumns->addAction( tr( "Filter by") );
    a->setEnabled( false );

    menuSelectColumns->addSeparator();

      // Columns
      // Without "Filename", "Poster", "IsViewed" and "IsFavourite"
    for( int column = 0; column < sourceModel->columnCount() - 2; ++column )
    {
        if( column == FilmItem::IsViewedColumn || column == FilmItem::IsFavouriteColumn )
        {
            continue;
        }

        QString columnTitle = sourceModel->headerData( column, Qt::Horizontal ).toString();
        a = menuSelectColumns->addAction( columnTitle, this, SLOT(CalculateOptions()) );
        actionsColumns.append( a );
        a->setCheckable( true );
        a->setData( column );
    }

      // Select/Unselect buttons
    menuSelectColumns->addSeparator();
    menuSelectColumns->addAction( tr( "Select all" ), this, SLOT( SelectAllOptions() ) );
    menuSelectColumns->addAction( tr( "Unselect all" ), this, SLOT( UnselectAllOptions() ) );
}


void SearchEdit::ShowMenu()
{
    menuSelectColumns->show(); // FIXME: For retrieving correct menu height at first call

    int posX = cursor().pos().x();
    int posY = cursor().pos().y() - menuSelectColumns->height();
    menuSelectColumns->exec( QPoint( posX, posY ) );
}


void SearchEdit::CalculateOptions()
{
    selectedColumns.clear();

    for( QAction* action : actionsColumns )
    {
        if( action->isChecked() )
        {
            selectedColumns.append( action->data().toInt() );
        }
    }

    if( !text().isEmpty() )
    {
        emit TextChanged( text(), selectedColumns );
    }
}


void SearchEdit::SetOptionsChecked( bool b )
{
    for( QAction* action : actionsColumns )
    {
        action->setChecked( b );
    }

    CalculateOptions();
}

