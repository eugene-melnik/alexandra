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
        Film010( const Film010& other ) = default;

        friend QDataStream& operator >> ( QDataStream& in, Film010& film );

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
        QString GetPosterName() const;
        bool    GetIsViewed() const     { return( isViewed ); }
        bool    GetIsFavourite() const  { return( isFavourite ); }


        QString GetScreenwriter() const { return( screenwriter ); }
        QString GetComposer() const { return( composer ); }

    private:
        QString id;
        quint8  section = 0; // unused
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
