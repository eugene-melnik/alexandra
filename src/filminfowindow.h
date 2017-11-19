/*************************************************************************************************
 *                                                                                                *
 *  file: filminfowindow.h                                                                        *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2017 Eugene Melnik <eugene@melnik.solutions>                               *
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

#ifndef FILMINFOWINDOW_H
#define FILMINFOWINDOW_H


#include "tools/mediainfo.h"
#include "alexandrasettings.h"
#include "ui_filminfowindow.h"


#include <QDialog>
#include <QMutex>


class FilmInfoWindow : public QDialog, protected Ui::FilmInfoWindow
{
    Q_OBJECT

    public:
        FilmInfoWindow( QWidget* parent = nullptr );
        virtual ~FilmInfoWindow();

        void LoadTechnicalInfoAsync( const QString& fileName );
        void LoadTechnicalInfo( const QString& fileName );

    signals:
        void FullInfoLoaded( const QString& shortInfo );

    private slots:
        void CopyToClipboard();

    private:
        QMutex loadInfoMutex;
};


#endif // FILMINFOWINDOW_H
