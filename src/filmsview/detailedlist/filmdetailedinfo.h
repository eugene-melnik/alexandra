/*************************************************************************************************
 *                                                                                                *
 *  file: filmdetailedinfo.h                                                                      *
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

#ifndef FILMDETAILEDINFO_H
#define FILMDETAILEDINFO_H


#include "filmslist/filmitem.h"
#include "ui_filmdetailedinfo.h"


#include <QPixmap>


class FilmDetailedInfo : public QWidget, protected Ui::FilmDetailedInfo
{
    Q_OBJECT

    public:
        explicit FilmDetailedInfo( QWidget* parent = nullptr ) : QWidget( parent )
        {
            setupUi( this );
        }

        void SetLazyLoading( bool lazy = true )
        {
            lazyLoading = lazy;
        }

        void SetFilm( const FilmItem* film )
        {
            this->film = film;

            if( !lazyLoading )
            {
                LoadFilmData();
            }
        }

    protected:
        void paintEvent( QPaintEvent* event )
        {
            if( !dataLoaded )
            {
                LoadFilmData();
            }

            QWidget::paintEvent( event );
        }

        void LoadFilmData()
        {
            QPixmap poster( film->GetPosterFilePath() );
            lPoster->setPixmap( poster.scaledToWidth( lPoster->maximumWidth(), Qt::SmoothTransformation ) );

            QString title = "<b> " + film->GetTitle() + "</b>";
            QString year = film->GetColumnData( FilmItem::YearColumn ).toString();

            if( !year.isEmpty() )
            {
                title += QString( " (%1)" ).arg( year );
            }

            lTitle->setText( title );
//            lRating->setPixmap( sourceModel->index( row, FilmItem::RatingColumn).data(Qt::DecorationRole).value<QPixmap>() );
            lGenres->setText( "<b>Genres:</b> " + film->GetColumnData( FilmItem::GenreColumn ).toString() );
            lActors->setText( "<b>Actors:</b> " + film->GetColumnData( FilmItem::StarringColumn).toString().left(200) + "..." );

            dataLoaded = true;
        }

    private:
        bool lazyLoading = false;
        bool dataLoaded = false;

        const FilmItem* film = nullptr;
};


#endif // FILMDETAILEDINFO_H

