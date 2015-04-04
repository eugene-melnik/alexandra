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

#include "alexandrasettings.h"
#include "film.h"

#include <QColor>
#include <QString>

class AbstractFilmsView
{
    public:
        virtual ~AbstractFilmsView() {}

        virtual void LoadSettings( AlexandraSettings* s ) = 0;
        virtual void ReloadSettings( AlexandraSettings* s ) = 0;
        virtual void SaveSettings( AlexandraSettings* s ) const = 0;

        virtual void AddItem( const Film& film ) = 0;
        virtual void AddItem( const Film& film, QColor background ) = 0;
        virtual void SetItem( int n, const Film* film, QColor background ) = 0;
        virtual void Clear() = 0;

        virtual void SelectItem( Film film ) = 0;
        virtual void SelectItem( QString title ) = 0;
        virtual void SelectRandomItem() = 0;

        virtual int GetItemsCount() const = 0;
        virtual int GetCurrentItemIndex() const = 0;

        virtual void SetCurrentItemIndex( int i ) = 0;

//    signals:
//        void ItemClicked( QString );
//        void ItemDoubleClicked( QString );
};

#endif // ABSTRACTFILMSVIEW_H
