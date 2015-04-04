
#ifndef FILMSVIEWGRIDITEM_H
#define FILMSVIEWGRIDITEM_H

#include <QLabel>
#include <QWidget>

#include "alexandrasettings.h"
#include "film.h"

class FilmViewGridItem : public QWidget
{
    Q_OBJECT

    public:
        FilmViewGridItem( const Film* film, AlexandraSettings* s, QWidget* parent = nullptr );

        QString GetTitle() const;

    private:
        QString titleText;
        QLabel* poster = nullptr;
        QLabel* title = nullptr;
};

#endif // FILMSVIEWGRIDITEM_H
