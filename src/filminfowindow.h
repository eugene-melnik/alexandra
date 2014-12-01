/*************************************************************************************************
 *                                                                                                *
 *  file: filminfowindow.h                                                                        *
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

#ifndef FILMINFOWINDOW_H
#define FILMINFOWINDOW_H

#include "ui_filminfowindow.h"

#include <MediaInfo/MediaInfo.h>
#include <QDialog>

class FilmInfoWindow : public QDialog, public Ui::FilmInfoWindow
{
    Q_OBJECT

    public:
        FilmInfoWindow( QWidget* parent = nullptr );
        ~FilmInfoWindow();

        void SetCurrentFile( const QString& fileName );
        const QString& GetShortTechInfo() const;

    private:
        void LoadShortInfo();

        MediaInfoLib::MediaInfo* mi = nullptr;

        QString shortInfo;
        QString fullInfo;
};

#endif // FILMINFOWINDOW_H
