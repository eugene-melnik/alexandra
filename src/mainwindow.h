#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include "filmslist.h"

#include "aboutwindow.h"
#include "addfilmwindow.h"
#include "editfilmwindow.h"
#include "settingswindow.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QSettings>

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

    public:
        MainWindow( QWidget* parent = nullptr );
        ~MainWindow();

    protected:
        void closeEvent( QCloseEvent* event );

    private slots:
        void SettingsChanged();

        void ShowEditFilmWindow();
        void RemoveFilm();

        void FilmSelected( const Film* f );
        void PlayFilm();

        void UpdateStatusBar();

    private:
        // Additional functions
        void ConfigureSubwindows();

        void SetDatabaseFileName();
        void LoadSettings();
        void SaveSettings();


        // Subwindows
        AboutWindow* aboutWindow = nullptr;
        AddFilmWindow* addFilmWindow = nullptr;
        EditFilmWindow* editFilmWindow = nullptr;
        SettingsWindow* settingsWindow = nullptr;

        // Variables
        QSettings* settings;
        QString databaseFileName;
};

#endif // MAINWINDOW_H
