#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "filmslist.h"
#include "aboutwindow.h"

#include <QMainWindow>
#include <QCloseEvent>

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

    public:
        MainWindow( QWidget* parent = nullptr );

    protected:
        void closeEvent( QCloseEvent* event );

    private:
        // functions
        void SetDataDirectory();
        void LoadSettings();
        void SaveSettings();

        // subwindows
        AboutWindow* aboutWindow = nullptr;

        // variables
        QString dataDirectory;
        FilmsList films;
};

#endif // MAINWINDOW_H
