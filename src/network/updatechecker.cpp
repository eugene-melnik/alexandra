/*************************************************************************************************
 *                                                                                                *
 *  file: updatechecker.cpp                                                                       *
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

#include "updatechecker.h"
#include "tools/debug.h"
#include "version.h"

#include <QJsonDocument>
#include <QJsonObject>


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

    QJsonObject json = QJsonDocument::fromJson( data ).object();

    QString latestVersion = json["latest_version"].toString();
    DebugPrint( "LATEST VERSION -- " + latestVersion );
    emit Loaded( latestVersion );

    DebugPrintFuncDone( "UpdateChecker::DataLoaded" );
}


void UpdateChecker::DataLoadError( const QString& e )
{
    DebugPrintFunc( "UpdateChecker::DataLoadError", e );
    Q_UNUSED(e)
}
