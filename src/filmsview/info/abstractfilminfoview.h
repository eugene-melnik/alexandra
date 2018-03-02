/*************************************************************************************************
 *                                                                                                *
 *  file: abstractfilminfoview.h                                                                  *
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

#ifndef ABSTRACTFILMINFOVIEW_H
#define ABSTRACTFILMINFOVIEW_H


#include <QModelIndex>


// TODO: comment

class AbstractFilmInfoView
{
    public:
        virtual ~AbstractFilmInfoView() {}

        virtual void ShowInformation( const QModelIndex& index ) = 0;
        virtual void Clear() = 0;
};


#endif // ABSTRACTFILMINFOVIEW_H

