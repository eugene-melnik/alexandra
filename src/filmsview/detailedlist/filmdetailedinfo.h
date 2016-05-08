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


#include <QPixmap>


class FilmDetailedInfo : public QWidget, protected Ui::FilmDetailedInfo
{
    Q_OBJECT

    public:
        explicit FilmDetailedInfo( QWidget* parent = nullptr )
            : QWidget( parent ), settings( AlexandraSettings::GetInstance() )
        {
            setupUi( this );
        }

        void SetTitle( const QString& title, int year = 0 )
        {
            QString s = QString( "<b>%1</b>" ).arg( title );

            if( year > 0 )
            {
                s += QString( " (%1)" ).arg( year );
            }

            lTitle->setText( s );
        }

        void SetRating( QPixmap rating )
        {
            lRating->setPixmap( rating );
        }

        void SetPoster( QPixmap poster )
        {
            int posterHeight = height() - 20; /// FIXME
            lPoster->setPixmap( poster.scaledToHeight(posterHeight, Qt::SmoothTransformation) );
        }

        void SetGenres( const QString& genres )
        {
            if( !genres.isEmpty() )
            {
                lGenres->setText( QString( "<b>%1:</b> %2" ).arg( tr("Genre") ).arg( genres ) );
            }
        }

        void SetStarring( const QString& starring )
        {
            if( !genres.starring() )
            {
                lActors->setText( QString( "<b>%1:</b> %2" ).arg( tr("Starring") ).arg( starring ) );
            }
        }

    protected:
        AlexandraSettings* settings;
};


#endif // FILMDETAILEDINFO_H

