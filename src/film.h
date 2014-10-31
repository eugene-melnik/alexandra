#ifndef FILM_H
#define FILM_H

#include <QDataStream>
#include <QString>
#include <QPixmap>

class Film
{
    public:
        Film();
        Film( const Film& other );

        bool operator > ( const Film& other ) const;
        bool operator < ( const Film& other ) const;
        bool operator == ( const Film& other ) const;

        friend QDataStream& operator << ( QDataStream& out, const Film& f );
        friend QDataStream& operator >> ( QDataStream& in, Film& f );

    //private:
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
        QPixmap poster;

        bool isViewed;
        bool isFavourite;
};

#endif // FILM_H
