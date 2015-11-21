/*************************************************************************************************
 *                                                                                                *
 *  file: filminfowidget.cpp                                                                      *
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

#include "filminfowidget.h"

FilmInfoWidget::FilmInfoWidget( QWidget* parent ) : QWidget( parent )
{
    setupUi( this );
}

void FilmInfoWidget::ShowFilmInfo( const Film* film )
{
    lFilmTitle->setText( film->GetTitle() );

    lOriginalTitle->setText( tr( "<b>Original title:</b> %1" ).arg( film->GetOriginalTitle() ) );
    lOriginalTitle->setVisible( !film->GetOriginalTitle().isEmpty() );

    lTagline->setText( tr( "<b>Tagline:</b> %1" ).arg( film->GetTagline() ) );
    lTagline->setVisible( !film->GetTagline().isEmpty() );

    lGenre->setText( tr( "<b>Genre:</b> %1" ).arg( film->GetGenre() ) );
    lGenre->setVisible( !film->GetGenre().isEmpty() );

    lYear->setText( tr( "<b>Year:</b> %1" ).arg( film->GetYear() ) );
    lYear->setVisible( film->GetYear() != 0 );

    lBudget->setText( tr( "<b>Budget:</b> %1" ).arg( film->GetBudgetStr() ) );
    lBudget->setVisible( !film->GetBudgetStr().isEmpty() );

    lCountry->setText( tr( "<b>Country:</b> %1" ).arg( film->GetCountry() ) );
    lCountry->setVisible( !film->GetCountry().isEmpty() );

    lScreenwriter->setText( tr( "<b>Screenwriter:</b> %1" ).arg( film->GetScreenwriter() ) );
    lScreenwriter->setVisible( !film->GetScreenwriter().isEmpty() );

    lDirector->setText( tr( "<b>Director:</b> %1" ).arg( film->GetDirector() ) );
    lDirector->setVisible( !film->GetDirector().isEmpty() );

    lProducer->setText( tr( "<b>Producer:</b> %1" ).arg( film->GetProducer() ) );
    lProducer->setVisible( !film->GetProducer().isEmpty() );

    lComposer->setText( tr( "<b>Composer:</b> %1" ).arg( film->GetComposer() ) );
    lComposer->setVisible( !film->GetComposer().isEmpty() );

    lStarring->setText( tr( "<b>Starring:</b> %1" ).arg( film->GetStarring() ) );
    lStarring->setVisible( !film->GetStarring().isEmpty() );

    lRating->setText( tr( "<b>Rating:</b> %1" ).arg( film->GetRatingStr() ) );
    lRating->setVisible( film->GetRating() != 1 );

    lDescription->setText( tr( "<b>Description:</b> %1" ).arg( film->GetDescription() ) );
    lDescription->setVisible( !film->GetDescription().isEmpty() );

    lTags->setText( tr( "<b>Tags:</b> %1" ).arg( film->GetTags() ) );
    lTags->setVisible( !film->GetTags().isEmpty() );
}

void FilmInfoWidget::Clear()
{
    lFilmTitle->clear();
    lOriginalTitle->clear();
    lTagline->clear();
    lGenre->clear();
    lYear->clear();
    lBudget->clear();
    lCountry->clear();
    lScreenwriter->clear();
    lDirector->clear();
    lProducer->clear();
    lComposer->clear();
    lStarring->clear();
    lRating->clear();
    lDescription->clear();
    lTags->clear();
}

void FilmInfoWidget::ShowEmptyDatabaseMessage() const
{
    QString title = tr( "Your database is empty" );
    lFilmTitle->setText( title );

    QString line1 = tr( "Hi! At this point your database is empty. It's time to fill it! To do this, use the following tools:" );
    lOriginalTitle->setText( line1 + "<br/><br/>" );
    lOriginalTitle->show();

    QString line2 = tr( "Adding movies one by one (menu \"Film\"→\"Add film\" or the \"Add\" button on the toolbar)." );
    lTagline->setText( "1. " + line2 );
    lTagline->show();

    QString line3 = tr( "Scan selected folders on the drive with the automatic addition of the selected films (menu \"Tools\"→\"Films scanner\" or button on the toolbar)." );
    lGenre->setText( "2. " + line3 );
    lGenre->show();

    QString line4 = tr( "If you already have filled database, just select it in the settings (\"Application\" tab)." );
    lCountry->setText( "3. " + line4 );
    lCountry->show();

    QString line5 = tr( "All of these tools have automatic information retrieval via the Internet, as well as loading a graphic poster for the film. Enjoy! :)" );
    lScreenwriter->setText( line5 );
    lScreenwriter->show();
}
