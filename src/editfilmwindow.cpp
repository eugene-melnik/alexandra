/*************************************************************************************************
 *                                                                                                *
 *  file: editfilmwindow.cpp                                                                      *
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

#include "editfilmwindow.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QProcessEnvironment>

EditFilmWindow::EditFilmWindow( AlexandraSettings* s, QWidget* parent ) : AddFilmWindow( s, parent )
{
    setWindowTitle( tr( "Edit film" ) );
    settings = s;
}

void EditFilmWindow::show( const Film* f )
{
    AddFilmWindow::show();

    if( f->GetIsPosterExists() ) {
        ePosterFileName->setText( settings->GetFilmsListPostersDir() + "/" + f->GetPosterName() );
    }

    filmId = f->GetId();
    eFilmFileName->setText( f->GetFileName() );
    eTitle->setText( f->GetTitle() );
    eOriginalTitle->setText( f->GetOriginalTitle() );
    eTagline->setText( f->GetTagline() );
    eYear->setText( f->GetYearStr() );
    eCountry->setText( f->GetCountry() );
    eGenre->setText( f->GetGenre() );
    cbRating->setCurrentIndex( f->GetRating() - 1 );
    eDirector->setText( f->GetDirector() );
    eProducer->setText( f->GetProducer() );
    tStarring->setPlainText( f->GetStarring() );
    tDescription->setPlainText( f->GetDescription() );
    eTags->setText( f->GetTags() );
    cIsViewed->setChecked( f->GetIsViewed() );
    cIsFavourite->setChecked( f->GetIsFavourite() );
}
