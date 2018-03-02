/*************************************************************************************************
 *                                                                                                *
 *  file: filesextensions.cpp                                                                     *
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

#include "filesextensions.h"

#include <QDir>
#include <QFileInfoList>


QStringList FilesExtensions::videos =
{
    "*.avi", "*.bik", "*.divx", "*.dv",  "*.flv",  "*.m1v",  "*.m2t", "*.m2ts", "*.m2v", "*.m4v",
    "*.mkv", "*.mov", "*.mp4",  "*.mpv", "*.mpeg", "*.mpg",  "*.mts", "*.ogm",  "*.ogv", "*.ogx",
    "*.rm",  "*.ts",  "*.vcd",  "*.vob", "*.vp8",  "*.webm", "*.wmv"
};

QStringList FilesExtensions::images =
{
    "*.bmp", "*.gif", "*.jpg", "*.jpeg", "*.png", "*.pbm", "*.pgm", "*.ppm", "*.xbm",  "*.xpm"
};


QString FilesExtensions::GetFilmExtensionsForFilter()
{
      // Creating a string with format "*.ext1 *.ext2 ... *.extN"
    QString res;

    for( QString str : videos )
    {
        res += str + " ";
    }

    return( res );
}


QString FilesExtensions::GetImageExtensionsForFilter()
{
    QString res;

    for( QString str : images )
    {
        res += str + " ";
    }

    return( res );
}


QString FilesExtensions::SearchForEponymousImage( const QString& fileName )
{
      // Searching for file "/path/to/film/filename.videoformat"
      // eponymous file "/path/to/film/filename.imageformat"

    QFileInfo fileInfo( fileName );

      // The problem with files that have in their names the square brackets, replaces them with '?'
      // TODO: Need to optimize
    QStringList filter( QString( fileInfo.completeBaseName() + ".*" ).replace( "[", "?" ).replace( "]", "?" ) );

      // Searching
    QFileInfoList eponymousFiles = QDir( fileInfo.absolutePath() ).entryInfoList( filter, QDir::Files );

    for( QFileInfo& info : eponymousFiles )
    {
        QString extension = "*." + info.suffix();

        if( images.contains( extension, Qt::CaseInsensitive ) )
        {
            return( info.absoluteFilePath() );
        }
    }

      // Nothing was found
    return( QString() );
}

