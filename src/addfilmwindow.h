/*************************************************************************************************
 *                                                                                                *
 *  file: addfilmwindow.h                                                                         *
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

#ifndef ADDFILMWINDOW_H
#define ADDFILMWINDOW_H

#include "ui_addfilmwindow.h"

#include "alexandrasettings.h"
#include "network/parsermanager.h"
#include "film.h"

#include <QCloseEvent>
#include <QDialog>
#include <QString>

class AddFilmWindow : public QDialog, public Ui::AddFilmWindow
{
    Q_OBJECT

    public:
        explicit AddFilmWindow( AlexandraSettings* settings, QWidget* parent = nullptr );
        virtual ~AddFilmWindow();

        void show();

    protected:
        void closeEvent( QCloseEvent* event );

        QString filmId;

    signals:
        void PosterMovingError();
        void Done( const Film& f );

    private slots:
        void OpenFilm();
        void OpenPosterFileClicked();
        void LoadInformation();
        void Save();

        void InformationLoaded( const Film& f, const QString& posterFileName );
        void InformationLoadError( const QString& e );

    private:
        void ClearFields();

        AlexandraSettings* settings = nullptr;
        ParserManager* parser = nullptr;
};

#endif // ADDFILMWINDOW_H
