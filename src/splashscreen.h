/*************************************************************************************************
 *                                                                                                *
 *  file: splashscreen.h                                                                          *
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

#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "ui_splashscreen.h"

#include <QDialog>

class SplashScreen : public QDialog, public Ui::SplashScreen
{
    Q_OBJECT

    public:
        SplashScreen( QWidget* parent = nullptr );

    public slots:
        void Close();
};

#endif // SPLASHSCREEN_H
