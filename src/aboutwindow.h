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

    protected:
        void showEvent( QShowEvent* event );
};

#endif // ABOUTWINDOW_H
