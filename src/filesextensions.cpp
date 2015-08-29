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

#include <QDir>
#include <QFileInfoList>

QString FilesExtensions::GetFilmExtensionsForFilter() const
{
    // Creating a string with format "*.ext1 *.ext2 ... *.extN"
    QString res;

    for( auto i = videos.begin(); i < videos.end(); i++ )
    {
        res += *i + " ";
    }

    return( res );
}

QString FilesExtensions::GetImageExtensionsForFilter() const
{
    QString res;

    for( auto i = images.begin(); i < images.end(); i++ )
    {
        res += *i + " ";
    }

    return( res );
}

QString FilesExtensions::SearchForEponymousImage( const QString& fileName ) const
{
    // Searching for file "/path/to/film/filename.videoformat"
    // eponymous file "/path/to/film/filename.imageformat"

    QFileInfo fileInfo( fileName );

    // The problem with files that have in their names the square brackets, replaces them with '?'
    // TODO: Need to optimize
    QStringList filter( QString( fileInfo.completeBaseName() + ".*" ).replace( "[", "?" ).replace( "]", "?" ) );

    // Searching
    QFileInfoList eponymousFiles = QDir( fileInfo.absolutePath() ).entryInfoList( filter, QDir::Files );

    if( eponymousFiles.size() > 1 )
    {
        for( auto i = eponymousFiles.begin(); i < eponymousFiles.end(); i++)
        {
            QString extension = "*." + i->suffix();

            if( images.contains( extension, Qt::CaseInsensitive ) )
            {
                return( i->absoluteFilePath() );
            }
        }
    }

    // If nothing was found
    return( QString() );
}
