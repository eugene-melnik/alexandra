/*************************************************************************************************
 *                                                                                                *
 *  file: editfilmwindow.cpp                                                                      *
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

#include "editfilmwindow.h"
#include "tools/debug.h"

#include <QProcessEnvironment>


EditFilmWindow::EditFilmWindow( QWidget* parent ) : AddFilmWindow( parent )
{
    setWindowTitle( tr( "Edit film" ) );
}


void EditFilmWindow::SetData( const FilmItem* film )
{
    QString posterFileName;

    if( film->GetIsPosterExists() )
    {
        posterFileName = film->GetPosterFilePath();
    }

    eFilmFileName->setText( film->GetFileName() );
    eTags->setPlainText( film->GetColumnData( FilmItem::TagsColumn ).toString() );
    cIsViewed->setChecked( film->GetColumnData( FilmItem::IsViewedColumn ).toBool() );
    cIsFavourite->setChecked( film->GetColumnData( FilmItem::IsFavouriteColumn ).toBool() );

    InformationLoaded( *film, posterFileName );
}

