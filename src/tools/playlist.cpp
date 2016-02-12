/*************************************************************************************************
 *                                                                                                *
 *  file: playlist.cpp                                                                            *
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

#include "playlist.h"
#include "tools/debug.h"

#include <QDir>
#include <QFile>
#include <QTextStream>


QString PlayList::CreateTempListM3U8() const
{
    DebugPrintFunc( "PlayList::CreateTempListM3U8" );

    if( pathes.isEmpty() )
    {
        return( QString() );
    }

    QString fileName( QDir::tempPath() + QString( "/tmpList%1.m3u8" ).arg( rand() ) );

    QFile fileToSave( fileName );
    fileToSave.open( QIODevice::WriteOnly );
    QTextStream streamToSave( &fileToSave );

    streamToSave << "#EXTM3U\n";  // m3u header

    for( const QString& p : pathes )
    {
        streamToSave << p.toUtf8() << "\n";
    }

    DebugPrintFuncDone( "PlayList::CreateTempListM3U8" );
    return( fileName );
}

