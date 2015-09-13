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

#include "searchedit.h"

SearchEdit::SearchEdit( QWidget* parent ) : QLineEdit( parent )
{
    connect( this, &SearchEdit::textChanged, this, [this] (const QString& s) { emit TextChanged( s, selectedFilters ); } );

    aOptions = addAction( QIcon( ":/tool/spanner" ), QLineEdit::TrailingPosition );
    aOptions->setToolTip( tr( "Select fields for filtration") );
    connect( aOptions, &QAction::triggered, this, &SearchEdit::ShowMenu );

    SetupMenu();
}

void SearchEdit::LoadSettings( const AlexandraSettings* s )
{
    selectedFilters = (FilterBy) s->GetQuickSearchFilter();

    for( FilterAction f : filterActions )
    {
        if( (f.filter & selectedFilters) != 0 )
        {
            f.action->setChecked( true );
        }
    }
}

void SearchEdit::SaveSettings( AlexandraSettings* s ) const
{
    s->SetQuickSearchFilter( selectedFilters );
}

void SearchEdit::SetupMenu()
{
    // Menu header
    mSelectFields = new SearchEditMenu( this );
    QAction* a = mSelectFields->addAction( tr( "Filter by:") );
    a->setEnabled( false );

    mSelectFields->addSeparator();

    // Menu options
    a = mSelectFields->addAction( tr( "Title" ), this, SLOT( CalculateOptions() ) );
    a->setCheckable( true );
    filterActions.append( { Title, a } );

    a = mSelectFields->addAction( tr( "Tags" ), this, SLOT( CalculateOptions() ) );
    a->setCheckable( true );
    filterActions.append( { Tags, a } );

    a = mSelectFields->addAction( tr( "Genre" ), this, SLOT( CalculateOptions() ) );
    a->setCheckable( true );
    filterActions.append( { Genre, a } );

    a = mSelectFields->addAction( tr( "Starring" ), this, SLOT( CalculateOptions() ) );
    a->setCheckable( true );
    filterActions.append( { Starring, a } );

    a = mSelectFields->addAction( tr( "Director" ), this, SLOT( CalculateOptions() ) );
    a->setCheckable( true );
    filterActions.append( { Director, a } );

    a = mSelectFields->addAction( tr( "Producer" ), this, SLOT( CalculateOptions() ) );
    a->setCheckable( true );
    filterActions.append( { Producer, a } );

    a = mSelectFields->addAction( tr( "Screenwriter" ), this, SLOT( CalculateOptions() ) );
    a->setCheckable( true );
    filterActions.append( { Screenwriter, a } );

    a = mSelectFields->addAction( tr( "Composer" ), this, SLOT( CalculateOptions() ) );
    a->setCheckable( true );
    filterActions.append( { Composer, a } );

    a = mSelectFields->addAction( tr( "Country" ), this, SLOT( CalculateOptions() ) );
    a->setCheckable( true );
    filterActions.append( { Country, a } );

    a = mSelectFields->addAction( tr( "Description" ), this, SLOT( CalculateOptions() ) );
    a->setCheckable( true );
    filterActions.append( { Description, a } );

    mSelectFields->addSeparator();

    // Menu "buttons"
    mSelectFields->addAction( tr( "Select all" ), this, SLOT( SelectAllOptions() ) );
    mSelectFields->addAction( tr( "Unselect all" ), this, SLOT( UnselectAllOptions() ) );
}

void SearchEdit::ShowMenu()
{
    mSelectFields->show(); // For retrieving correct menu height at first call

    int posX = cursor().pos().x();
    int posY = cursor().pos().y() - mSelectFields->height();
    mSelectFields->exec( QPoint( posX, posY ) );
}

void SearchEdit::CalculateOptions()
{
    selectedFilters = None;

    for( FilterAction f : filterActions )
    {
        if( f.action->isChecked() )
        {
            selectedFilters = (FilterBy) (selectedFilters | f.filter);
        }
    }

    if( !this->text().isEmpty() )
    {
        emit TextChanged( this->text(), selectedFilters );
    }
}

void SearchEdit::SetOptionsChecked( bool b )
{
    for( FilterAction f : filterActions )
    {
        f.action->setChecked( b );
    }

    CalculateOptions();
}
