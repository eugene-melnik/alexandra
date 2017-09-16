/*************************************************************************************************
 *                                                                                                *
 *  file: networkrequest.h                                                                        *
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

#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H

#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPointer>
#include <QUrl>


class NetworkRequest : public QObject
{
    Q_OBJECT

    public:
        NetworkRequest() = default;
        ~NetworkRequest();

        void SetProxy( QNetworkProxy* proxy );
        void ClearProxy();

        void run( const QUrl& url );
        QByteArray runSync( const QUrl& url );

        void Abort();

    signals:
        void Progress( quint64 received, quint64 total );
        void DataLoaded( const QByteArray& data );
        void DataLoadError( const QString& e );

    private slots:
        QNetworkReply* MakeRequest( const QUrl& url );

        void CheckForRedirect();
        void ReadyRead();
        void Finished();

    private:
        QNetworkAccessManager accessManager;
        QPointer<QNetworkReply> reply;

        QNetworkProxy* proxy = nullptr;

        QByteArray data;
};


#endif // NETWORKREQUEST_H
