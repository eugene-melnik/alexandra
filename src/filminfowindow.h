/*************************************************************************************************
 *                                                                                                *
 *  file: filminfowindow.h                                                                        *
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

#ifndef FILMINFOWINDOW_H
#define FILMINFOWINDOW_H

#include "alexandrasettings.h"
#include "ui_filminfowindow.h"
#include "mediainfo.h"

#include <QDialog>
#include <QMutex>
#include <QString>

class FilmInfoWindow : public QDialog, public Ui::FilmInfoWindow
{
    Q_OBJECT

    public:
        FilmInfoWindow( QWidget* parent = nullptr );
        virtual ~FilmInfoWindow();

        void LoadTechnicalInfoAsync( const QString& fileName );
        void LoadTechnicalInfo( const QString& fileName );

        void show();

    signals:
        void ShortInfoLoaded( const QString& shortInfo );
        void FullInfoLoaded( const QString& shortInfo );

    private slots:
        void ShowFullInfo( const QString& s ) { eTechInfo->setPlainText( s ); }
        void CopyToClipboard();
        void ActuallyLoad();

    private:
        AlexandraSettings* settings = nullptr;
        QString savedFileName;
        QMutex loadInfoMutex;
};

#endif // FILMINFOWINDOW_H
