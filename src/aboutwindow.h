/*************************************************************************************************
 *                                                                                                *
 *  file: aboutwindow.h                                                                           *
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

#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H


#include "ui_aboutwindow.h"


#include <QDialog>
#include <QMessageBox>


class AboutWindow : public QDialog, protected Ui::AboutWindow
{
    Q_OBJECT

    public:
        explicit AboutWindow( QWidget* parent = nullptr );

        void show();

    public slots:
        void AboutQt() { QMessageBox::aboutQt( this->parentWidget() ); }

    signals:
        void UpdateAvailable( const QString& link );

    private slots:
        void CompareVersions( const QString& latestVersion );
};


#endif // ABOUTWINDOW_H

