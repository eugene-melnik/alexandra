#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "ui_aboutwindow.h"

#include <QDialog>
#include <QShowEvent>

class AboutWindow : public QDialog, public Ui::AboutWindow
{
    Q_OBJECT

    public:
        AboutWindow( QWidget* parent = nullptr );

    public slots:
        void AboutQt();

    protected:
        void showEvent( QShowEvent* event );
};

#endif // ABOUTWINDOW_H
