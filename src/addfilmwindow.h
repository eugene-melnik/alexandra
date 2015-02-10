/*************************************************************************************************
 *                                                                                                *
 *  file: addfilmwindow.h                                                                         *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014 Eugene Melnik <jeka7js@gmail.com>                                          *
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

#ifndef ADDFILMWINDOW_H
#define ADDFILMWINDOW_H

#include "ui_addfilmwindow.h"

#include "alexandrasettings.h"
#include "film.h"

#include <QDialog>
#include <QShowEvent>

class AddFilmWindow : public QDialog, public Ui::AddFilmWindow
{
    Q_OBJECT

    public:
        AddFilmWindow( AlexandraSettings* settings, QWidget* parent = nullptr );

    protected:
        void showEvent( QShowEvent* event );

        QString filmId;

    signals:
        void PosterMovingError();
        void Done( Film f );

    private slots:
        void OpenFilmFileClicked();
        void OpenPosterFileClicked();
        void OkButtonClicked();

    private:
        void ClearFields();

        AlexandraSettings* settings = nullptr;
};

#endif // ADDFILMWINDOW_H
