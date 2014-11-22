/*************************************************************************************************
 *                                                                                                *
 *  file: film.h                                                                                  *
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

#ifndef FILM_H
#define FILM_H

#include <QDataStream>
#include <QString>

class Film
{
    public:
        Film();
        Film( const Film& other );

        // Functions needed for comparation (sorting)
        bool operator > ( const Film& other ) const;
        bool operator < ( const Film& other ) const;
        bool operator == ( const Film& other ) const;

        // Functions needed for serialization
        friend QDataStream& operator << ( QDataStream& out, const Film& f );
        friend QDataStream& operator >> ( QDataStream& in, Film& f );

        // Getters
        const QString& GetFileName() const;
        const QString& GetTitle() const;
        const QString& GetOriginalTitle() const;
        const QString& GetTagline() const;
        const QString& GetGenre() const;
        const QString& GetCountry() const;
        quint16 GetYear() const;
        QString GetYearStr() const;
        const QString& GetDirector() const;
        const QString& GetProducer() const;
        const QString& GetStarring() const;
        const QString& GetDescription() const;
        quint8 GetRating() const;
        QString GetRatingStr() const;
        const QString& GetPosterName() const;

        bool GetIsViewed() const;
        QString GetIsViewedSign() const;
        bool GetIsFavourite() const;
        QString GetIsFavouriteSign() const;

        // Setters
        void SetFileName( const QString& s );
        void SetTitle( const QString& s );
        void SetOriginalTitle( const QString& s );
        void SetTagline( const QString& s );
        void SetGenre( const QString& s );
        void SetCountry( const QString& s );
        void SetYear( quint16 n );
        bool SetYearFromStr( const QString& s );
        void SetDirector( const QString& s );
        void SetProducer( const QString& s );
        void SetStarring( const QString& s );
        void SetDescription( const QString& s );
        void SetRating( quint8 n );
        bool SetRatingFromStr( const QString& s );
        void SetPosterName( const QString& s );

        void SetIsViewed( bool b );
        void SetIsFavourite( bool b );

        void SetNewData( const Film& other );

    private:
        QString fileName;
        QString title;
        QString originalTitle;
        QString tagline;
        QString genre;
        QString country;
        quint16 year;
        QString director;
        QString producer;
        QString starring;
        QString description;
        quint8  rating;
        QString posterName;

        bool isViewed;
        bool isFavourite;
};

#endif // FILM_H
