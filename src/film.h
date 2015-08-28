/*************************************************************************************************
 *                                                                                                *
 *  file: film.h                                                                                  *
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
        bool operator > ( const Film& other ) const { return( title > other.title ); }
        bool operator < ( const Film& other ) const { return( title < other.title ); }
        bool operator == ( const Film& other ) const { return( title == other.title ); }

        // Functions needed for serialization
        friend QDataStream& operator << ( QDataStream& out, const Film& f );
        friend QDataStream& operator >> ( QDataStream& in, Film& f );

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
        QString GetPosterName() const       { return( GetId() ); }
        bool    GetIsViewed() const         { return( isViewed ); }
        QString GetIsViewedSign() const     { return( isViewed ? "+" : "-" ); }
        bool    GetIsFavourite() const      { return( isFavourite ); }
        QString GetIsFavouriteSign() const  { return( isFavourite ? "+" : "-" ); }


        QString GetScreenwriter() const { return( screenwriter ); }
        QString GetComposer() const { return( composer ); }
#ifdef QT_DEBUG
        QString DebugGetAllFields() const;
#endif // QT_DEBUG

        // Setters
        void SetId( const QString& s )          { id = s; }
        void SetFileName( const QString& s )    { fileName = s; }
        void SetTitle( const QString& s )       { title = s; }
        void SetOriginalTitle( const QString& s ) { originalTitle = s; }
        void SetTagline( const QString& s )     { tagline = s; }
        void SetGenre( const QString& s )       { genre = s; }
        void SetCountry( const QString& s )     { country = s; }
        void SetYear( quint16 n )               { year = n; }
        bool SetYearFromStr( const QString& s );
        void SetDirector( const QString& s )    { director = s; }
        void SetProducer( const QString& s )    { producer = s; }
        void SetStarring( const QString& s )    { starring = s; }
        void SetDescription( const QString& s ) { description = s; }
        void SetRating( quint8 n )              { rating = n; }
        bool SetRatingFromStr( const QString& s );
        void SetTags( const QString& s )        { tags = s; }

        bool SetBudgetFromStr( const QString& s );
        void IncViewsCounter()              { viewsCounter++; }
        void SetViewCounter( int count )    { viewsCounter = count; }
        void SetIsPosterExists( bool b )    { isPosterExists = b; }
        void SetIsViewed( bool b )          { isViewed = b; }
        void SetIsFavourite( bool b )       { isFavourite = b; }

        void SetScreenwriter( QString s )   { screenwriter = s; }
        void SetComposer( QString s )       { composer = s; }

        void SetNewData( const Film& other );

        static QString GetRandomHash();

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
        /* QString additionalData; (deprecated) */

        quint16 viewsCounter = 0;

        bool isPosterExists = false;
        bool isViewed = false;
        bool isFavourite = false;

        // Additional fields ('additionalData')
        int additionalDataVersion = 0x01;

        QString screenwriter;
        QString composer;
};

#endif // FILM_H
