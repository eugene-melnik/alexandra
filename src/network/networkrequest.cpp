/*************************************************************************************************
 *                                                                                                *
 *  file: networkrequest.cpp                                                                      *
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

#include "networkrequest.h"
#include "version.h"
#include "tools/debug.h"


void NetworkRequest::run( const QUrl& url )
{
    DebugPrintFunc( "NetworkRequest::run", url.toString() );

    data.clear();
    reply = MakeRequest( url );

    connect( reply.data(), &QNetworkReply::downloadProgress, this, [this] (quint64 received, quint64 total) { emit Progress( received, total ); } );
    connect( reply.data(), &QNetworkReply::readyRead, this, &NetworkRequest::ReadyRead );
    connect( reply.data(), &QNetworkReply::finished, this, &NetworkRequest::Finished );
    connect( reply.data(), &QNetworkReply::finished, reply, &QObject::deleteLater );

    DebugPrintFuncDone( "NetworkRequest::run" );
}


QByteArray NetworkRequest::runSync( const QUrl& url )
{
    DebugPrintFunc( "NetworkRequest::runSync", url.toString() );

    data.clear();
    reply = MakeRequest( url );

    QEventLoop loop;
    connect( reply.data(), &QNetworkReply::downloadProgress, this, [this] (quint64 received, quint64 total) { emit Progress( received, total ); } );
    connect( reply.data(), &QNetworkReply::readyRead, this, &NetworkRequest::ReadyRead );
    connect( reply.data(), &QNetworkReply::finished, this, &NetworkRequest::CheckForRedirect );
    connect( reply.data(), &QNetworkReply::finished, &loop, &QEventLoop::quit );
    loop.exec();

    DebugPrint( QString( "Loaded: %1 bytes" ).arg( data.size() ) );
    DebugPrintFuncDone( "NetworkRequest::runSync" );

    reply->deleteLater();
    return( data );
}


void NetworkRequest::Abort()
{
    if( !reply.isNull() )
    {
        reply->abort();
    }
}


QNetworkReply* NetworkRequest::MakeRequest( const QUrl& url )
{
    QString userAgent = QString( "Alexandra/%1.%2 (%3)" ).arg( Alexandra::verMajor )
                                                         .arg( Alexandra::verMinor )
                                                   #ifdef Q_OS_LINUX
                                                         .arg( "GNU/Linux" ).toUtf8();
                                                   #else
                                                         .arg( "MS Windows" ).toUtf8();
                                                   #endif

    QNetworkRequest request( url );
    request.setRawHeader( "User-Agent", userAgent.toUtf8() );

    return( accessManager.get( request ) );
}


void NetworkRequest::CheckForRedirect()
{
    DebugPrintFunc( "NetworkRequest::CheckForRedirect" );
    QVariant redirectionTarget = reply->attribute( QNetworkRequest::RedirectionTargetAttribute );

    if( !redirectionTarget.isNull() )
    {
        QUrl newUrl = reply->url().resolved( redirectionTarget.toUrl() );
        DebugPrint( "Redirected to: " + newUrl.toString() );
        runSync( newUrl );
    }

    DebugPrintFuncDone( "NetworkRequest::CheckForRedirect" );
}


void NetworkRequest::ReadyRead()
{
    data.append( reply->readAll() );
}


void NetworkRequest::Finished()
{
    DebugPrintFunc( "NetworkRequest::Finished" );

    CheckForRedirect();

    if( reply->error() )
    {
        DebugPrint( "Error: " + reply->errorString() );
        emit DataLoadError( reply->errorString() );
    }
    else
    {
        DebugPrint( QString( "Loaded: %1 bytes" ).arg( data.size() ) );
        emit DataLoaded( data );
    }

    DebugPrintFuncDone( "NetworkRequest::Finished" );
}

