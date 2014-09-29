#ifndef FILM_H
#define FILM_H

#include <QString>
#include <QImage>

class Film
{
    public:
        Film();

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
        QImage  poster;
};

#endif FILM_H
