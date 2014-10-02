#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "ui_aboutwindow.h"

#include <QDialog>

class AboutWindow : public QDialog, public Ui::AboutWindow
{
    Q_OBJECT

    public:
        AboutWindow( QWidget* parent = nullptr );
};

#endif // ABOUTWINDOW_H
