/*************************************************************************************************
 *                                                                                                *
 *  file: filmscannerworker.cpp                                                                   *
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
#include "filmscannerworker.h"

#include <QDir>
#include <QFileInfo>
#include <memory>

void FilmScannerWorker::run()
{
    isCanceled = false;
    isTerminated = false;

    QList<QString>* result = new QList<QString>();

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

QList<QString>* FilmScannerWorker::ScanDirectory( const QString& dir )
{
    QList<QString>* result = new QList<QString>();
    QFileInfoList files = QDir( dir ).entryInfoList( FilesExtensions().GetFilmExtensionsForDirFilter() );

    for( QList<QFileInfo>::iterator i = files.begin(); i < files.end(); i++ )
    {
        if( isCanceled ) break; // Scanning canceled

        result->append( i->absoluteFilePath() );
        emit IncFoundedTotal();
    }

    return( result );
}

QList<QString>* FilmScannerWorker::ScanDirectoryRecursive( const QString& dir )
{
    // Scan files in directory
    QList<QString>* result = ScanDirectory( dir );

    // Scan files in subdirectories recursively
    QFileInfoList files = QDir( dir ).entryInfoList( QDir::NoDotAndDotDot | QDir::Dirs );

    for( QList<QFileInfo>::iterator i = files.begin(); i < files.end(); i++ )
    {
        if( isCanceled ) break; // Scanning canceled

        std::unique_ptr<QList<QString>> subdirFiles( ScanDirectoryRecursive( i->absoluteFilePath() ) );
        result->append( *subdirFiles );
    }

    return( result );
}
