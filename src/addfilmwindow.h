#ifndef ADDFILMWINDOW_H
#define ADDFILMWINDOW_H

#include "ui_addfilmwindow.h"

#include "film.h"

#include <QDialog>
#include <QAbstractButton>

class AddFilmWindow : public QDialog, public Ui::AddFilmWindow
{
    Q_OBJECT

    public:
        AddFilmWindow( QWidget* parent = nullptr );

    signals:
        void AddFilm( Film f );

    private slots:
        void OkButtonClicked();
        void CancelButtonClicked();

    private:
        void ConfigureCBRating();
        void ClearFields();
};

#endif // ADDFILMWINDOW_H
