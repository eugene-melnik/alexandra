
#ifndef FILMSCANNERWINDOW_H
#define FILMSCANNERWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include <QList>
#include <QString>
#include <QStringList>

#include "film.h"
#include <ui_filmscannerwindow.h>

class FilmScannerWindow : public QDialog, public Ui::FilmScannerWindow
{
    Q_OBJECT

    public:
        FilmScannerWindow( QWidget* parent = nullptr );

        void show( QStringList* l );

    signals:
        void AddFilms( const QList<Film>* );

    protected:
        void closeEvent( QCloseEvent* event );

    private slots:
        void SelectDirectory();

        void Scan();

        void SelectAll();
        void UnselectAll();
        void InvertSelection();
        void CalculateSelected();

        void AddSelected();

    private:
        QList<QString>* ScanDirectory( QString dir );
        QList<QString>* ScanDirectoryRecursive( QString dir );

        QStringList* existsFileNames = nullptr;

        const QStringList filter = { "*.avi", "*.flv", "*.m2ts", "*.m4v", "*.mkv", "*.mov",
                                     "*.mp4", "*.mpeg", "*.mpg", "*.mts", "*.ogm", "*.ogv",
                                     "*.rm", "*.ts", "*.wmv" };
};

#endif // FILMSCANNERWINDOW_H
