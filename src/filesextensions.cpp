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

QString FilesExtensions::SearchForEponymousImage( QString fileName ) const
{
    QFileInfo fileInfo( fileName );
    QStringList filter( fileInfo.completeBaseName() + ".*" );
    QFileInfoList eponymousFiles = QDir( fileInfo.absolutePath() ).entryInfoList( filter );

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

    return( QString() );
}
