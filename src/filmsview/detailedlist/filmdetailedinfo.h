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


#include "alexandrasettings.h"
#include "filmslist/filmitem.h"
#include "tools/debug.h"
#include "ui_filmdetailedinfo.h"


#include <QPainter>
#include <QPixmap>


class FilmDetailedInfo : public QWidget, protected Ui::FilmDetailedInfo
{
    Q_OBJECT

    public:
        explicit FilmDetailedInfo( QWidget* parent = nullptr )
            : QWidget( parent ), settings( AlexandraSettings::GetInstance() )
        {
            this->setupUi( this );
        }

        void SetPoster( QPixmap poster )
        {
            int posterHeight = height() - 20; /// FIXME
            this->lPoster->setPixmap( poster.scaledToHeight(posterHeight, Qt::SmoothTransformation) );
        }

        void SetTitle( const QString& title, int year = 0 )
        {
            QString s = QString( "<b>%1</b>" ).arg( title );

            if( year > 0 )
            {
                s += QString( " (%1)" ).arg( year );
            }

            this->lTitle->setText( s );
        }

        void SetViewed( QPixmap viewed ) { SetFieldPixmap( lViewed, viewed ); }
        void SetFavourite( QPixmap favourite ) { SetFieldPixmap( lFavourite, favourite ); }
        void SetRating( QPixmap rating ) { SetFieldPixmap( lRating, rating ); }

        void SetCountry( const QString& country ) { SetFieldData( lCountry, tr( "Country" ), country ); }
        void SetGenres( const QString& genres ) { SetFieldData( lGenre, tr( "Genre" ), genres ); }
        void SetDirector( const QString& director ) { SetFieldData( lDirector, tr( "Director" ), director ); }
        void SetStarring( const QString& starring ) { SetFieldData( lStarring, tr( "Starring" ), starring ); }

    protected:
        void SetFieldData( QLabel* field, const QString& title, const QString& data )
        {
            if( !data.isEmpty() )
            {
                field->setText( QString( "<b>%1:</b> %2" ).arg( title ).arg( data ) );
            }
            else
            {
                field->clear();
            }
        }

        void SetFieldPixmap( QLabel* field, const QPixmap& pixmap )
        {
            if( !pixmap.isNull() )
            {
                field->setPixmap( pixmap );
            }
            else
            {
                field->clear();
            }
        }

        AlexandraSettings* settings;
};


#endif // FILMDETAILEDINFO_H

