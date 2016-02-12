/*************************************************************************************************
 *                                                                                                *
 *  file: playlist.h                                                                              *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2016 Eugene Melnik <jeka7js@gmail.com>                                     *
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

#ifndef PLAYLIST_H
#define PLAYLIST_H


#include <QStringList>


class PlayList
{
    public:
        PlayList() = delete;
        PlayList( const QStringList& p ) : pathes( p ) {}

        void AddPath( const QString& s ) { pathes.append( s ); }
        void AddPathes( const QStringList& s ) { pathes.append( s ); }
        void Clear() { pathes.clear(); }

        QString CreateTempListM3U8() const;

    private:
        QStringList pathes;
};


#endif // PLAYLIST_H

