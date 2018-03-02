/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewcontextmenu.h                                                                  *
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

#ifndef FILMSVIEWCONTEXTMENU_H
#define FILMSVIEWCONTEXTMENU_H


#include <QAction>
#include <QMenu>


#include "filmslist/filmitem.h"


class FilmsViewContextMenu : public QMenu
{
    Q_OBJECT

    public:
        explicit FilmsViewContextMenu( QWidget* parent = nullptr );

        void SetReadOnlyMode( bool readOnly ) { isReadWrite = !readOnly; }

        void SetupMenuState( const FilmItem* film );

    signals:
        void actionPlay();
        void actionAddToList();
        void actionShowInfo();
        void actionIsViewed( bool );
        void actionIsFavourite( bool );
        void actionEdit();
        void actionRemove();
        void actionRemoveFile();

    private:
        QAction* cmaPlay = nullptr;
        QAction* cmaAddToList = nullptr;
        QAction* cmaShowInfo = nullptr;
        QAction* cmaIsViewed = nullptr;
        QAction* cmaIsFavourite = nullptr;
        QAction* cmaEdit = nullptr;
        QAction* cmaRemove = nullptr;
        QAction* cmaRemoveFile = nullptr;

        bool isReadWrite = true;
};


#endif // FILMSVIEWCONTEXTMENU_H

