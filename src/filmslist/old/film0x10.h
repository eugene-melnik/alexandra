/*************************************************************************************************
 *                                                                                                *
 *  file: film.h                                                                                  *
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

#ifndef Film010_H
#define Film010_H


#include <QDataStream>


class Film010
{
    public:
        Film010() = default;

        friend QDataStream& operator >> ( QDataStream& in, Film010& film )
        {
            QString additionalData;

            in >> film.id
               >> film.section
               >> film.fileName
               >> film.title
               >> film.originalTitle
               >> film.tagline
               >> film.genre
               >> film.country
               >> film.year
               >> film.director
               >> film.producer
               >> film.starring
               >> film.budget
               >> film.description
               >> film.rating
               >> film.ageRestrictions
               >> film.tags
               >> additionalData
               >> film.viewsCounter
               >> film.isPosterExists
               >> film.isViewed
               >> film.isFavourite;

              // Additional fields
            int version; // useless
            QByteArray dataArray = QByteArray::fromHex( additionalData.toLatin1() );
            QDataStream stream( dataArray );
            stream >> version >> film.screenwriter >> film.composer;

            return( in );
        }

          // Getters
        QString GetId() const           { return( id ); }
        QString GetFileName() const     { return( fileName ); }
        QString GetTitle() const        { return( title ); }
        QString GetOriginalTitle() const { return( originalTitle ); }
        QString GetTagline() const      { return( tagline ); }
        QString GetGenre() const        { return( genre ); }
        QString GetCountry() const      { return( country ); }
        int     GetYear() const               { return( year ); }
        QString GetDirector() const     { return( director ); }
        QString GetProducer() const     { return( producer ); }
        QString GetStarring() const     { return( starring ); }
        QString GetDescription() const  { return( description ); }
        double  GetRating() const       { return( rating * 1.0 ); }
        QString GetTags() const         { return( tags ); }
        double  GetBudget() const       { return( budget ); }
        int     GetViewsCounter() const { return( viewsCounter ); }
        bool    GetIsPosterExists() const { return( isPosterExists ); }
        bool    GetIsViewed() const     { return( isViewed ); }
        bool    GetIsFavourite() const  { return( isFavourite ); }
        QString GetScreenwriter() const { return( screenwriter ); }
        QString GetComposer() const { return( composer ); }

        QString GetPosterName() const
        {
            if( isPosterExists )
            {
                return( GetId() );
            }
            else
            {
                return( QString() );
            }
        }

    private:
        QString id;
        quint8  section; // unused
        QString fileName;
        QString title;
        QString originalTitle;
        QString tagline;
        QString genre;
        QString country;
        quint16 year = 0;
        QString director;
        QString producer;
        QString starring;
        double  budget = 0;
        QString description;
        quint8  rating = 1;
        QString ageRestrictions; // unused
        QString tags;

        quint16 viewsCounter = 0;

        bool isPosterExists = false;
        bool isViewed = false;
        bool isFavourite = false;

          // Additional fields (QByteArray::toHex() format)
          // Compatibility with older versions
        QString screenwriter;
        QString composer;
};


#endif // Film010_H

