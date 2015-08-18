/*************************************************************************************************
 *                                                                                                *
 *  file: filesextensions.cpp                                                                     *
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

#include "filesextensions.h"

QString FilesExtensions::GetFilmExtensionsForFilter()
{
    QString res;

    for( auto i = videos.begin(); i < videos.end(); i++ )
    {
        res += *i + " ";
    }

    return( res );
}

QString FilesExtensions::GetImageExtensionsForFilter()
{
    QString res;

    for( auto i = images.begin(); i < images.end(); i++ )
    {
        res += *i + " ";
    }

    return( res );
}

const QStringList& FilesExtensions::GetFilmExtensionsForDirFilter()
{
    return( videos );
}

const QStringList &FilesExtensions::GetImageExtensionsForDirFilter()
{
    return( images );
}
