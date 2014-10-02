#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "filmslist.h"
#include "aboutwindow.h"

#include <QMainWindow>

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

    public:
        MainWindow( QWidget* parent = nullptr );
        ~MainWindow();

    private:
        // subwindows
        AboutWindow* aboutWindow = nullptr;

        // variables
        FilmsList films;
};

#endif // MAINWINDOW_H
