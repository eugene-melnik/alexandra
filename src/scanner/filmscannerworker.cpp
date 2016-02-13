/*************************************************************************************************
 *                                                                                                *
 *  file: filmscannerworker.cpp                                                                   *
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

#include "tools/filesextensions.h"
#include "filmscannerworker.h"

#include <QDir>
#include <QFileInfo>
#include <memory>


void FilmScannerWorker::run()
{
    isCanceled = false;
    isTerminated = false;

    QStringList result;

    if( isRecursive )
    {
        result = ScanDirectoryRecursive( dir );
    }
    else
    {
        result = ScanDirectory( dir );
    }

    if( !isTerminated ) // If window isn't closed
    {
        emit Scanned( result );
    }
}


QStringList FilmScannerWorker::ScanDirectory( const QString& dir )
{
    QStringList result;
    QFileInfoList files = QDir( dir ).entryInfoList( FilesExtensions().GetFilmExtensionsForDirFilter() );

    for( const QFileInfo& fileInfo : files )
    {
        if( isCanceled ) break;

        result.append( fileInfo.absoluteFilePath() );
        emit IncFoundedTotal();
    }

    return( result );
}


QStringList FilmScannerWorker::ScanDirectoryRecursive( const QString& dir )
{
      // Scan files in directory
    QStringList result = ScanDirectory( dir );

      // Scan files in subdirectories recursively
    QFileInfoList files = QDir( dir ).entryInfoList( QDir::NoDotAndDotDot | QDir::Dirs );

    for( const QFileInfo& filmInfo : files )
    {
        if( isCanceled ) break;

        result.append( ScanDirectoryRecursive( filmInfo.absoluteFilePath() ) );
    }

    return( result );
}

