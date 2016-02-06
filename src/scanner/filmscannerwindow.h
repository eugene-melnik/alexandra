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

#include <QDialog>
#include <QList>
#include <QStringList>

#include "filmscannerworker.h"
#include "filmscanneraddworker.h"
#include "ui_filmscannerwindow.h"

class FilmScannerWindow : public QDialog, protected Ui::FilmScannerWindow
{
    Q_OBJECT

    public:
        explicit FilmScannerWindow( QWidget* parent = nullptr );
        virtual ~FilmScannerWindow();

        void show( QStringList* l );

    signals:
        void AddFilms( const QList<Film>* );

    protected:
        void reject() override { close(); }
        void closeEvent( QCloseEvent* event ) override;

    private slots:
        void SelectDirectory();

        void Scan();
        void IncFoundedTotal();
        void ShowFounded( QList<QString>* fileNames );

        void SelectAll();
        void UnselectAll();
        void InvertSelection();
        void CalculateSelected();

        void AddSelected();
        void FilmsCreated( QList<Film> films );

    private:
        QStringList* existsFileNames = nullptr;
        FilmScannerWorker* filmScannerWorker = nullptr;
        QList<Film>* newFilms = nullptr;
        int newFilmsCount;
};

#endif // FILMSCANNERWINDOW_H
