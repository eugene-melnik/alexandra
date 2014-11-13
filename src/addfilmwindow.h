#ifndef ADDFILMWINDOW_H
#define ADDFILMWINDOW_H

#include "ui_addfilmwindow.h"

#include "film.h"

#include <QDialog>
#include <QSettings>
#include <QShowEvent>

class AddFilmWindow : public QDialog, public Ui::AddFilmWindow
{
    Q_OBJECT

    public:
        AddFilmWindow( QSettings* settings, QWidget* parent = nullptr );

    protected:
        void showEvent( QShowEvent* event );

    signals:
        void Done( Film f );

    private slots:
        void OpenFilmFileClicked();
        void OpenPosterFileClicked();
        void OkButtonClicked();

    private:
        void ClearFields();

        QSettings* settings = nullptr;
};

#endif // ADDFILMWINDOW_H
