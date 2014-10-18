#ifndef ADDFILMWINDOW_H
#define ADDFILMWINDOW_H

#include "ui_addfilmwindow.h"

#include <QDialog>

class AddFilmWindow : public QDialog, public Ui::AddFilmWindow
{
    Q_OBJECT

    public:
        AddFilmWindow( QWidget* parent = nullptr );
};

#endif // ADDFILMWINDOW_H
