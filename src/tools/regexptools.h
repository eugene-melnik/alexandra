/*************************************************************************************************
 *                                                                                                *
 *  file: regexptools.h                                                                           *
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

#ifndef REGEXPTOOLS_H
#define REGEXPTOOLS_H

#include <QRegExp>

class RegExpTools
{
    public:
        static void SimplifyText( QString& str );

        static QString ParseList( const QString& str, QRegExp& reList, QRegExp& reItem, quint16 stripTo = 0 );
        static QString ParseItem( const QString& str, QRegExp& reItem );
};

#endif // REGEXPTOOLS_H
