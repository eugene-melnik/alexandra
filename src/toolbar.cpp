/*************************************************************************************************
 *                                                                                                *
 *  file: toolbar.cpp                                                                             *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014 Eugene Melnik <jeka7js@gmail.com>                                          *
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

#include "toolbar.h"

#include <QIcon>

ToolBar::ToolBar( QWidget* parent ) : QToolBar( parent )
{
    setContextMenuPolicy( Qt::PreventContextMenu ); // Disable context menu

    // Add film
    tbaAdd = new QAction( QIcon( ":/tool/add" ), tr( "Add" ), this );
    addAction( tbaAdd );
    connect( tbaAdd, SIGNAL( triggered() ), this, SLOT( actionAddTriggered() ) );

    // Edit film
    tbaEdit = new QAction( QIcon( ":/tool/edit" ), tr( "Edit" ), this );
    addAction( tbaEdit );
    connect( tbaEdit, SIGNAL( triggered() ), this, SLOT( actionEditTriggered() ) );

    // Remove film
    tbaRemove = new QAction( QIcon( ":/tool/delete" ), tr( "Remove" ), this );
    addAction( tbaRemove );
    connect( tbaRemove, SIGNAL( triggered() ), this, SLOT( actionRemoveTriggered() ) );

    addSeparator();

    // Random
    tbaRandom = new QAction( QIcon( ":/tool/random" ), tr( "Random" ), this );
    addAction( tbaRandom );
    connect( tbaRandom, SIGNAL( triggered() ), this, SLOT( actionRandomTriggered() ) );

    // Search
    tbaSearch = new QAction( QIcon( ":/tool/find" ), tr( "Search" ), this );
    addAction( tbaSearch );
    connect( tbaSearch, SIGNAL( triggered() ), this, SLOT( actionSearchTriggered() ) );

    addSeparator();

    // Exit
    tbaExit = new QAction( QIcon( ":/action/exit" ), tr( "Exit" ), this );
    addAction( tbaExit );
    connect( tbaExit, SIGNAL( triggered() ), this, SLOT( actionExitTriggered() ) );
}

ToolBar::~ToolBar()
{
    delete tbaAdd;
    delete tbaEdit;
    delete tbaRemove;
    delete tbaRandom;
    delete tbaSearch;
    delete tbaExit;
}

void ToolBar::LoadSettings( QSettings* s )
{
    setToolButtonStyle( (Qt::ToolButtonStyle)s->value( "MainWindow/ToolbarStyle", Qt::ToolButtonFollowStyle ).toInt() );
}

void ToolBar::SetEditFunctionsEnabled( bool b )
{
    tbaAdd->setEnabled( b );
    tbaEdit->setEnabled( b );
    tbaRemove->setEnabled( b );
}

void ToolBar::actionAddTriggered()
{
    emit actionAdd();
}

void ToolBar::actionEditTriggered()
{
    emit actionEdit();
}

void ToolBar::actionRemoveTriggered()
{
    emit actionRemove();
}

void ToolBar::actionRandomTriggered()
{
    emit actionRandom();
}

void ToolBar::actionSearchTriggered()
{
    emit actionSearch();
}

void ToolBar::actionExitTriggered()
{
    emit actionExit();
}
