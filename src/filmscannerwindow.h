/*************************************************************************************************
 *                                                                                                *
 *  file: filmscannerwindow.h                                                                     *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>                                     *
 *                                                                                                *
 *  Alexandra is free software; you can redistribute it and/or modify it under the terms of the   *
 *  GNU General Public License as published by the Free Software Foundation; either version 2 of  *
 *  the License, or (at your option) any later version.                                           *
 *                                                                                                *
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     *
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     *
 *  See the GNU General Public License for more details.                                          *
 *                                                                                                *
 *  You should have received a copy of the GNU General Public License along with this program.    *
 *  If not, see <http://www.gnu.org/licenses/>.                                                   *
 *                                                                                                *
  *************************************************************************************************/

#ifndef FILMSCANNERWINDOW_H
#define FILMSCANNERWINDOW_H

#include <QCloseEvent>
#include <QDialog>
#include <QList>
#include <QString>
#include <QStringList>

#include "alexandrasettings.h"
#include "film.h"
#include "ui_filmscannerwindow.h"

class FilmScannerWindow : public QDialog, public Ui::FilmScannerWindow
{
    Q_OBJECT

    public:
        FilmScannerWindow( AlexandraSettings* s, QWidget* parent = nullptr );

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

        AlexandraSettings* settings = nullptr;
        QStringList* existsFileNames = nullptr;

        const QStringList filter = { "*.avi", "*.flv", "*.m2ts", "*.m4v", "*.mkv", "*.mov",
                                     "*.mp4", "*.mpeg", "*.mpg", "*.mts", "*.ogm", "*.ogv",
                                     "*.rm", "*.ts", "*.wmv" };
};

#endif // FILMSCANNERWINDOW_H
