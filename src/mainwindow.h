#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include "filmslist.h"

#include "aboutwindow.h"
#include "addfilmwindow.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QTableWidgetItem>

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

    public:
        MainWindow( QWidget* parent = nullptr );

    protected:
        void closeEvent( QCloseEvent* event );

    private slots:
        void AboutQt();
        void AddFilm( Film f );

        void FilmSelected( QTableWidgetItem* item );

    private:
        // Additional functions
        void ConfigureToolbar();
        void ConfigureSubwindows();
        void CreateFilmsTable();

        void SetDataDirectory();
        void LoadSettings();
        void SaveSettings();

        void UpdateFilmsTable();
        void UpdateStatusBar();

        // Subwindows
        AboutWindow* aboutWindow = nullptr;
        AddFilmWindow* addFilmWindow = nullptr;

        // Variables
        FilmsList* films = nullptr;
        QString dataDirectory;
};

#endif // MAINWINDOW_H
