/*************************************************************************************************
 *                                                                                                *
 *  file: toolbar.cpp                                                                             *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2017 Eugene Melnik <eugene@melnik.solutions>                               *
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
#include "toolbar.h"

#include <QIcon>


ToolBar::ToolBar( QWidget* parent ) : QToolBar( parent )
{
    // Disable context menu
    setContextMenuPolicy( Qt::PreventContextMenu );

    // Items
    tbaAdd = addAction( QIcon( ":/tool/add" ), tr("Add") );
    connect( tbaAdd, &QAction::triggered, this, &ToolBar::actionAdd );

    tbaEdit = addAction( QIcon( ":/tool/edit" ), tr("Edit") );
    connect( tbaEdit, &QAction::triggered, this, &ToolBar::actionEdit );

    tbaRemove = addAction( QIcon( ":/tool/delete" ), tr("Remove") );
    connect( tbaRemove, &QAction::triggered, this, &ToolBar::actionRemove );

    addSeparator();

    tbaFilmScanner = addAction( QIcon( ":/window/film-scanner" ), tr("Film scanner") );
    connect( tbaFilmScanner, &QAction::triggered, this, &ToolBar::actionFilmScanner );

    addSeparator();

    tbaRandom = addAction( QIcon( ":/tool/random" ), tr("Random") );
    connect( tbaRandom, &QAction::triggered, this, &ToolBar::actionRandom );

    tbaSearch = addAction( QIcon( ":/tool/find" ), tr("Search") );
    connect( tbaSearch, &QAction::triggered, this, &ToolBar::actionSearch );

    addSeparator();

    tbaExit = addAction( QIcon( ":/action/exit" ), tr("Exit") );
    connect( tbaExit, &QAction::triggered, this, &ToolBar::actionExit );
}


void ToolBar::LoadSettings()
{
    setToolButtonStyle( Qt::ToolButtonStyle( AlexandraSettings::GetInstance()->GetMainWindowToolbarStyle() ) );
}


void ToolBar::SetAllFunctionsEnabled( bool b )
{
    tbaAdd->setEnabled( b );
    tbaEdit->setEnabled( b );
    tbaRemove->setEnabled( b );
    tbaFilmScanner->setEnabled( b );
    tbaRandom->setEnabled( b );
    tbaSearch->setEnabled( b );
}


void ToolBar::SetEmptyDatabaseMode( bool b )
{
    tbaEdit->setDisabled( b );
    tbaRemove->setDisabled( b );
    tbaRandom->setDisabled( b );
    tbaSearch->setDisabled( b );
}


void ToolBar::SetReadOnlyMode( bool b )
{
    tbaAdd->setDisabled( b );
    tbaEdit->setDisabled( b );
    tbaRemove->setDisabled( b );
    tbaFilmScanner->setDisabled( b );
}

