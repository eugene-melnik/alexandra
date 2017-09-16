/*************************************************************************************************
 *                                                                                                *
 *  file: abstractparser.h                                                                        *
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

#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H

#include "alexandrasettings.h"
#include "filmslist/filmitem.h"
#include "tools/debug.h"
#include "network/networkrequest.h"

// TODO: need to comment //


class AbstractParser : public QObject
{
    Q_OBJECT

    public:
        AbstractParser()
        {
            if( AlexandraSettings::GetInstance()->GetParsersProxyEnabled() )
            {
                QString host = AlexandraSettings::GetInstance()->GetParsersProxyHostname();
                int port = AlexandraSettings::GetInstance()->GetParsersProxyPort();
                QString username = AlexandraSettings::GetInstance()->GetParsersProxyUsername();
                QString password = AlexandraSettings::GetInstance()->GetParsersProxyPassword();

                QNetworkProxy* proxy = new QNetworkProxy( QNetworkProxy::HttpProxy, host, port, username, password );

                this->request.SetProxy( proxy );
            }

            connect( &request, &NetworkRequest::DataLoaded, this, &AbstractParser::DataLoaded );
            connect( &request, &NetworkRequest::DataLoadError, this, [this] (const QString& e) { emit Error( e ); } );
            connect( &request, &NetworkRequest::Progress, this, [this] (quint64 received, quint64 total) { emit Progress( received, total ); } );
        }

        virtual void SearchFor( const QString& title, const QString& year = QString() )
        {
            DebugPrintFunc( "AbstractParser::SearchFor", title );
            QUrl url( searchUrlWithYear.arg( QUrl::toPercentEncoding( title ).data() ).arg( year ) );

            if( year.isEmpty() )
            {
                url = searchUrl.arg( QUrl::toPercentEncoding( title ).data() );
            }

            request.run( url );
        }

        virtual void SyncSearchFor( FilmItem* filmSaveTo, QUrl* posterUrlSaveTo,
                                    const QString& title, const QString& year = QString() )
        {
            DebugPrintFunc( "AbstractParser::SyncSearchFor", title );
            QUrl url( searchUrlWithYear.arg( title ).arg( year ) );

            if( year.isEmpty() )
            {
                url = searchUrl.arg( title );
            }

            QByteArray data = request.runSync( url );
            QUrl posterUrl = Parse( data );

            if( filmSaveTo != nullptr )
            {
                  // We must to save these fields
                QString title = filmSaveTo->GetTitle();
                QString fileName = filmSaveTo->GetFileName();

                *filmSaveTo = film;

                filmSaveTo->SetColumnData( FilmItem::FileNameColumn, fileName );
                filmSaveTo->SetColumnData( FilmItem::TitleColumn, title );
            }

            *posterUrlSaveTo = posterUrl;
        }

        void Abort() { request.Abort(); }

        virtual ~AbstractParser() = default;

    protected slots:
        // This slot must be overridden by concrete parser
        virtual QUrl Parse( const QByteArray& data ) = 0;

        virtual void DataLoaded( const QByteArray& data )
        {
            DebugPrintFunc( "AbstractParser::DataLoaded", data.size() );
            Parse( data );
        }

    signals:
        void Progress( quint64 received, quint64 total );
        void Loaded( const FilmItem& film, const QUrl& posterUrl );
        void Error( const QString& e);

    protected:
        QString searchUrlWithYear;
        QString searchUrl;

        NetworkRequest request;
        FilmItem film;
};


#endif // ABSTRACTPARSER_H
