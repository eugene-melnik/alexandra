/*************************************************************************************************
 *                                                                                                *
 *  file: abstractfilmsview.h                                                                     *
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

#ifndef ABSTRACTFILMSVIEW_H
#define ABSTRACTFILMSVIEW_H


#include <QColor>
#include <QModelIndex>
#include <QStringList>

// TODO: docs

class AbstractFilmsView
{
    public:
        virtual ~AbstractFilmsView() {}

        virtual void LoadSettings() {}
        virtual void ReloadSettings() {}
        virtual void SaveSettings() const {}

        virtual int GetRowCount() = 0;
        virtual QModelIndex GetCurrentIndex() = 0;
        virtual QModelIndexList GetSelectedItemsList() = 0;

        virtual void SetCurrentIndex( const QModelIndex& index ) = 0;
        virtual void SetCurrentRow( int row ) = 0;

        virtual void ScrollToCurrent() = 0;

//    signals:
//        void CurrentChanged( const QModelIndex& );
//        void ContextMenuRequested( const QPoint& );
};


#endif // ABSTRACTFILMSVIEW_H

