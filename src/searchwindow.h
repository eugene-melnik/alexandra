/*************************************************************************************************
 *                                                                                                *
 *  file: searchwindow.h                                                                          *
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

#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include "ui_searchwindow.h"

#include <QDialog>

class SearchWindow : public QDialog, public Ui::SearchWindow
{
    Q_OBJECT

    public:
        SearchWindow( QWidget* parent = nullptr );

    private:
        void ConfigureTable();
};

#endif // SEARCHWINDOW_H
