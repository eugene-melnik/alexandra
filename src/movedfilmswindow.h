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

#include <QCloseEvent>
#include <QDialog>
#include <QList>

#include "alexandrasettings.h"
#include "film.h"
#include "ui_movedfilmswindow.h"

class MovedFilmsWindow : public QDialog, public Ui::MovedFilmsWindow
{
    Q_OBJECT

    public:
        MovedFilmsWindow( AlexandraSettings* s, QWidget* parent = nullptr );

        void show( QList<Film*>* f );

    protected:
        void closeEvent( QCloseEvent* event );

    signals:
        void FilmsMoved();

    private slots:
        void SelectDirectory();

        void Scan();

        void SelectAll();
        void UnselectAll();
        void InvertSelection();
        void CalculateSelected();

        void MoveSelected();

    private:
        QList<QString>* ScanDirectory( QString dir );
        QList<QString>* ScanDirectoryRecursive( QString dir );

        AlexandraSettings* settings = nullptr;
        QList<Film*>* films = nullptr;

        const QStringList filter = { "*.avi", "*.flv", "*.m2ts", "*.m4v", "*.mkv", "*.mov",
                                     "*.mp4", "*.mpeg", "*.mpg", "*.mts", "*.ogm", "*.ogv",
                                     "*.rm", "*.ts", "*.wmv" };
};

#endif // MOVEDFILMSWINDOW_H
