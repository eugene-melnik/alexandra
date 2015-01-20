/*************************************************************************************************
 *                                                                                                *
 *  file: toolbar.h                                                                               *
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

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QAction>
#include <QToolBar>
#include <QSettings>

class ToolBar : public QToolBar
{
    Q_OBJECT

    public:
        ToolBar( QWidget* parent = nullptr );

        void LoadSettings( QSettings* s );

        void SetEditFunctionsEnabled( bool b );

    signals:
        void actionAdd();
        void actionEdit();
        void actionRemove();
        void actionRandom();
        void actionSearch();
        void actionExit();

    private slots:
        void actionAddTriggered();
        void actionEditTriggered();
        void actionRemoveTriggered();
        void actionRandomTriggered();
        void actionSearchTriggered();
        void actionExitTriggered();

    private:
        QAction* tbaAdd = nullptr;
        QAction* tbaEdit = nullptr;
        QAction* tbaRemove = nullptr;
        QAction* tbaRandom = nullptr;
        QAction* tbaSearch = nullptr;
        QAction* tbaExit = nullptr;
};

#endif // TOOLBAR_H
