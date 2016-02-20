/*************************************************************************************************
 *                                                                                                *
 *  file: updatechecker.cpp                                                                       *
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

#include "updatechecker.h"
#include "tools/debug.h"
#include "version.h"

#include <QRegExp>


UpdateChecker::UpdateChecker()
{
    connect( &request, &NetworkRequest::DataLoaded, this, &UpdateChecker::DataLoaded );
    connect( &request, &NetworkRequest::DataLoadError, this, &UpdateChecker::DataLoadError );
}


void UpdateChecker::Run()
{
    QUrl url( Alexandra::linkUpdates );
    request.run( url );
}


void UpdateChecker::DataLoaded( const QByteArray& data )
{
    DebugPrintFunc( "UpdateChecker::DataLoaded", data.size() );

    QString str( data );

    QRegExp reCounterUrl( "<counter_lnk>(.*)</counter_lnk>" );
    reCounterUrl.setMinimal( true );
    reCounterUrl.indexIn( str );
    DebugPrint( "COUNTER URL -- " + reCounterUrl.cap(1) );

#ifndef QT_DEBUG
      // Don't count launches of debug version of an app
    request.runSync( reCounterUrl.cap(1) );
#endif

    QRegExp reLatestVersion( "<latest_ver>(.*)</latest_ver>" );
    reLatestVersion.setMinimal( true );
    reLatestVersion.indexIn( str );

    DebugPrint( "LATEST VERSION -- " + reLatestVersion.cap(1) );
    DebugPrintFuncDone( "UpdateChecker::DataLoaded" );

    emit Loaded( reLatestVersion.cap(1) );
}


void UpdateChecker::DataLoadError( const QString& e )
{
    DebugPrintFunc( "UpdateChecker::DataLoadError", e );
    Q_UNUSED(e)
}
