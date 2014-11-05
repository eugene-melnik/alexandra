#ifndef EDITFILMWINDOW_H
#define EDITFILMWINDOW_H

#include "addfilmwindow.h"

class EditFilmWindow : public AddFilmWindow
{
    Q_OBJECT

    public:
        EditFilmWindow( QWidget* parent = nullptr );

    public slots:
        void show( const Film* f );
};

#endif // EDITFILMWINDOW_H
