/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewgriditem.cpp                                                                   *
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

#include "effects/effectdropshadow.h"
#include "filmsviewgriditem.h"

#include <QFileInfo>
#include <QPixmap>
#include <QString>
#include <QVBoxLayout>

FilmViewGridItem::FilmViewGridItem( const Film* film, AlexandraSettings* s, QWidget* parent )
    : QWidget( parent )
{
    titleText = film->GetTitle();

    // Film information
    if( s->GetGridShowTooltip() == true )
    {
        QString tooltip = film->GetTitle() + "<br/>";
        tooltip += tr( "<b>Original title:</b> %1" ).arg( film->GetOriginalTitle() ) + "<br/>";
        tooltip += tr( "<b>Genre:</b> %1" ).arg( film->GetGenre() ) + "<br/>";
        tooltip += tr( "<b>Year:</b> %1" ).arg( film->GetYearStr() ) + "<br/>";
        tooltip += tr( "<b>Country:</b> %1" ).arg( film->GetCountry() ) + "<br/>";
        tooltip += tr( "<b>Director:</b> %1" ).arg( film->GetDirector() ) + "<br/>";
        tooltip += tr( "<b>Producer:</b> %1" ).arg( film->GetProducer() ) + "<br/>";
        tooltip += tr( "<b>Starring:</b> %1" ).arg( film->GetStarring() ) + "<br/>";
        tooltip += tr( "<b>Rating:</b> %1" ).arg( film->GetRatingStr() ) + "<br/>";
        tooltip += tr( "<b>Description:</b> %1" ).arg( film->GetDescription() ) + "<br/>";
        tooltip += tr( "<b>Tags:</b> %1" ).arg( film->GetTags() ) + "<br/>";
        setToolTip( tooltip );
    }

    // Film poster
    poster = new QLabel( this );
    poster->setAlignment( Qt::AlignCenter );

    QString posterFilePath = s->GetPostersDirPath() + "/" + film->GetPosterName();

    if( !film->GetIsPosterExists() || !QFileInfo( posterFilePath ).exists() )
    {
        posterFilePath = ":/standart-poster";
    }

    int itemSize = s->GetGridItemSize();
    poster->setPixmap( QPixmap( posterFilePath ).scaledToHeight( itemSize, Qt::SmoothTransformation ) );
    poster->setGraphicsEffect( new EffectDropShadow( 0, 0, 10, poster ) );

    // Film title
    title = new QLabel( this );
    title->setAlignment( Qt::AlignCenter );
    title->setStyleSheet( QString( "font-size: %1px" ).arg( s->GetGridFontSize() ) );
    title->setWordWrap( true );

    int textLenght = s->GetGridCutTextAfter();

    if( film->GetTitle().length() > textLenght )
    {
        title->setText( film->GetTitle().left( textLenght ) + "..." );
    }
    else
    {
        title->setText( film->GetTitle() );
    }

    // Main layout
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->insertWidget( 0, poster );
    layout->insertWidget( 1, title );
    setLayout( layout );
}

QString FilmViewGridItem::GetTitle() const
{
    return( titleText );
}
