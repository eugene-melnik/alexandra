/*************************************************************************************************
 *                                                                                                *
 *  file: filesextensions.h                                                                       *
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

#ifndef FILESEXTENSIONS_H
#define FILESEXTENSIONS_H

#include <QStringList>

// This class contains a listing of all currently supported file formats

class FilesExtensions
{
    public:
        FilesExtensions() = default;

        QString GetFilmExtensionsForFilter() const;
        QString GetImageExtensionsForFilter() const;

        const QStringList& GetFilmExtensionsForDirFilter() const { return( videos ); }
        const QStringList& GetImageExtensionsForDirFilter() const { return( images ); }

        QString SearchForEponymousImage( const QString& fileName ) const;

    private:
        const QStringList videos = { "*.avi",  "*.bik", "*.divx", "*.dv",  "*.flv",  "*.m1v",  "*.m2t",
                                     "*.m2ts", "*.m2v", "*.m4v",  "*.mkv", "*.mov",  "*.mp4",  "*.mpv",
                                     "*.mpeg", "*.mpg", "*.mts",  "*.ogm", "*.ogv",  "*.ogx",  "*.rm",
                                     "*.ts",   "*.vcd", "*.vob",  "*.vp8", "*.webm", "*.wmv" };

        const QStringList images = { "*.bmp", "*.gif", "*.jpg", "*.jpeg", "*.png",
                                     "*.pbm", "*.pgm", "*.ppm", "*.xbm",  "*.xpm" };
};

#endif // FILESEXTENSIONS_H
