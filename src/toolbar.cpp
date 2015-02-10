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
    // Disable context menu
    setContextMenuPolicy( Qt::PreventContextMenu );

    // Items
    tbaAdd = addAction( QIcon( ":/tool/add" ), tr( "Add" ), this, SLOT( actionAddTriggered()) );
    tbaEdit = addAction( QIcon( ":/tool/edit" ), tr( "Edit" ), this, SLOT( actionEditTriggered() ) );
    tbaRemove = addAction( QIcon( ":/tool/delete" ), tr( "Remove" ), this, SLOT( actionRemoveTriggered() ) );
    addSeparator();
    tbaRandom = addAction( QIcon( ":/tool/random" ), tr( "Random" ), this, SLOT( actionRandomTriggered() ) );
    tbaSearch = addAction( QIcon( ":/tool/find" ), tr( "Search" ), this, SLOT( actionSearchTriggered() ) );
    addSeparator();
    tbaExit = addAction( QIcon( ":/action/exit" ), tr( "Exit" ), this, SLOT( actionExitTriggered() ) );
}

void ToolBar::LoadSettings( const AlexandraSettings* s )
{
    setToolButtonStyle( (Qt::ToolButtonStyle) s->GetMainWindowToolbarStyle() );
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
