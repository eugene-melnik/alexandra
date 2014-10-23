#ifndef ADDFILMWINDOW_H
#define ADDFILMWINDOW_H

#include "ui_addfilmwindow.h"

#include "film.h"

#include <QDialog>
#include <QShowEvent>

class AddFilmWindow : public QDialog, public Ui::AddFilmWindow
{
    Q_OBJECT

    public:
        AddFilmWindow( QWidget* parent = nullptr );

    protected:
        void showEvent( QShowEvent* event );

    signals:
        void AddFilm( Film f );

    private slots:
        void OpenFilmFile();
        void OpenPosterFile();
        void OkButtonClicked();

    private:
        void ConfigureCBRating();
        void ClearFields();
};

#endif // ADDFILMWINDOW_H
