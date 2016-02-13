/*************************************************************************************************
 *                                                                                                *
 *  file: filesextensions.h                                                                       *
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

#ifndef FILESEXTENSIONS_H
#define FILESEXTENSIONS_H


#include <QStringList>


/**
 * @brief This class contains a listing of all currently supported file formats
 */

class FilesExtensions
{
    public:
        static QString GetFilmExtensionsForFilter();
        static QString GetImageExtensionsForFilter();

        static const QStringList& GetFilmExtensionsForDirFilter() { return( videos ); }
        static const QStringList& GetImageExtensionsForDirFilter() { return( images ); }

        static QString SearchForEponymousImage( const QString& fileName );

    private:
        static QStringList videos;
        static QStringList images;
};


#endif // FILESEXTENSIONS_H

