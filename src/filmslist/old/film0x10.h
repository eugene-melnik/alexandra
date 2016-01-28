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
        Film010();
        Film010( const Film010& other ) = default;

        friend QDataStream& operator >> ( QDataStream& in, Film010& film );

          // Getters
        const QString& GetId() const        { return( id ); }
        const QString& GetFileName() const  { return( fileName ); }

        const QString& GetTitle() const     { return( title ); }
        const QString& GetOriginalTitle() const { return( originalTitle ); }
        const QString& GetTagline() const   { return( tagline ); }
        const QString& GetGenre() const     { return( genre ); }
        const QString& GetCountry() const   { return( country ); }
        quint16 GetYear() const             { return( year ); }
        QString GetYearStr() const          { return( QString::number( year ) ); }
        const QString& GetDirector() const  { return( director ); }
        const QString& GetProducer() const  { return( producer ); }
        const QString& GetStarring() const  { return( starring ); }
        const QString& GetDescription() const { return( description ); }
        quint8 GetRating() const            { return( rating ); }
        QString GetRatingStr() const        { return( QString( "%1/10" ).arg( rating ) ); }
        const QString &GetTags() const      { return( tags ); }

        QString GetBudgetStr() const;
        int     GetViewsCounter() const     { return( viewsCounter ); }
        bool    GetIsPosterExists() const   { return( isPosterExists ); }
        QString GetPosterName() const;
        bool    GetIsViewed() const         { return( isViewed ); }
        QString GetIsViewedSign() const     { return( isViewed ? "+" : "-" ); }
        bool    GetIsFavourite() const      { return( isFavourite ); }
        QString GetIsFavouriteSign() const  { return( isFavourite ? "+" : "-" ); }


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
