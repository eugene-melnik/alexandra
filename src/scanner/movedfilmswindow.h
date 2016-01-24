/*************************************************************************************************
 *                                                                                                *
 *  file: movedfilmswindow.h                                                                      *
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

#ifndef MOVEDFILMSWINDOW_H
#define MOVEDFILMSWINDOW_H

#include <QDialog>
#include <QList>

#include "filmslist/film.h"
#include "filmscannerworker.h"
#include "ui_movedfilmswindow.h"

class MovedFilmsWindow : public QDialog, protected Ui::MovedFilmsWindow
{
    Q_OBJECT

    public:
        explicit MovedFilmsWindow( QWidget* parent = nullptr );
        virtual ~MovedFilmsWindow();

        void show( QList<Film*>* f );

    signals:
        void FilmsMoved();

    protected:
        void reject() override { close(); }
        void closeEvent( QCloseEvent* event ) override;

    private slots:
        void SelectDirectory();

        void Scan();
        void ShowFounded( QList<QString>* fileNames );

        void SelectAll();
        void UnselectAll();
        void InvertSelection();
        void CalculateSelected();

        void MoveSelected();

    private:
        QList<Film*>* films = nullptr;
        FilmScannerWorker* filmScannerWorker = nullptr;
};

#endif // MOVEDFILMSWINDOW_H
