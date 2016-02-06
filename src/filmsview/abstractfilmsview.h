/*************************************************************************************************
 *                                                                                                *
 *  file: abstractfilmsview.h                                                                     *
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

#ifndef ABSTRACTFILMSVIEW_H
#define ABSTRACTFILMSVIEW_H

#include <QColor>
#include <QModelIndex>
#include <QStringList>

class AbstractFilmsView
{
    public:
        virtual ~AbstractFilmsView() {}

        virtual void LoadSettings() {}
        virtual void ReloadSettings() {}
        virtual void SaveSettings() const {}

        virtual void ScrollToCurrentItem() = 0; ///

        virtual QModelIndex GetCurrentIndex() = 0;
        virtual QModelIndexList GetSelectedItemsList() = 0;

//        virtual void SetCurrentItemTo( const Film film ) = 0;
//        virtual void SelectRandomItem() = 0; /* TODO: Maybe need to move to another place */
//        virtual QStringList GetSelectedItemsList() const = 0;

//    signals:
//        void CurrentChanged( const QModelIndex& );
//        void ContextMenuRequested( const QPoint& );
};

#endif // ABSTRACTFILMSVIEW_H
