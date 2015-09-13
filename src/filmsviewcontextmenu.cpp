/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewcontextmenu.cpp                                                                *
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

#include "filmsviewcontextmenu.h"

#include <QFileInfo>

FilmsViewContextMenu::FilmsViewContextMenu( QWidget* parent ) : QMenu( parent )
{
    // Play
    cmaPlay = addAction( QIcon( ":/action/play"), tr( "Play" ), this, SLOT( actionPlaySlot() ) );

    cmaAddToList = addAction( QIcon( ":/action/add-to-list"), tr( "Add to playlist" ) );
    connect( cmaAddToList, &QAction::triggered, this, [this] { emit actionAddToList(); } );
    addSeparator();

    // Show technical information
    cmaShowInfo = addAction( QIcon( ":/window/about"), tr( "Show technical information" ), this, SLOT( actionShowInfoSlot() ) );
    addSeparator();

    // IsViewed
    cmaIsViewed = addAction( tr( "Is viewed" ) );
    cmaIsViewed->setCheckable( true );
    connect( cmaIsViewed, &QAction::triggered, this, &FilmsViewContextMenu::actionIsViewedSlot );

    // IsFavourite
    cmaIsFavourite = addAction( tr( "Is favourite" ) );
    cmaIsFavourite->setCheckable( true );
    connect( cmaIsFavourite, &QAction::triggered, this, &FilmsViewContextMenu::actionIsFavouriteSlot );

    // Separator
    addSeparator();

    // Edit
    addAction( QIcon( ":/tool/edit" ), tr( "Edit" ), this, SLOT( actionEditSlot() ) );

    // Remove
    addAction( QIcon( ":/tool/delete" ), tr( "Remove" ), this, SLOT( actionRemoveSlot() ) );

    // Separator
    addSeparator();

    // Remove file
    addAction( tr( "Remove file" ), this, SLOT( actionRemoveFileSlot() ) );
}

void FilmsViewContextMenu::SetState( const Film* film )
{
    if( QFileInfo( film->GetFileName() ).exists() )
    {
        cmaPlay->setEnabled( true );
        cmaShowInfo->setEnabled( true );
    }
    else
    {
        cmaPlay->setEnabled( false );
        cmaShowInfo->setEnabled( false );
    }

    cmaIsViewed->setChecked( film->GetIsViewed() );
    cmaIsFavourite->setChecked( film->GetIsFavourite() );
}

void FilmsViewContextMenu::actionPlaySlot()
{
    emit actionPlay();
}

void FilmsViewContextMenu::actionShowInfoSlot()
{
    emit actionShowInfo();
}

void FilmsViewContextMenu::actionIsViewedSlot( bool b )
{
    emit actionIsViewed( b );
}

void FilmsViewContextMenu::actionIsFavouriteSlot( bool b )
{
    emit actionIsFavourite( b );
}

void FilmsViewContextMenu::actionEditSlot()
{
    emit actionEdit();
}

void FilmsViewContextMenu::actionRemoveSlot()
{
    emit actionRemove();
}

void FilmsViewContextMenu::actionRemoveFileSlot()
{
    emit actionRemoveFile();
}
