/*************************************************************************************************
 *                                                                                                *
 *  file: omdbparser.h                                                                            *
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

#ifndef OMDBPARSER_H
#define OMDBPARSER_H

#include "../abstractparser.h"
#include "../networkrequest.h"

class OmdbParser : public QObject, public AbstractParser
{
    Q_OBJECT

    public:
        OmdbParser();

        void SearchFor( const QString& title, const QString& year = QString() );
        void SyncSearchFor( Film* filmSaveTo, QString* posterFileNameSaveTo,
                            const QString& title, const QString& year = QString() );

    signals:
        void Progress( quint64 received, quint64 total );
        void Loaded( const Film& f, const QString& posterFileName );
        void Error( const QString& e );

    private slots:
        void DataLoaded( const QByteArray& data );

        QString Parse( const QByteArray& data );

    private:
        NetworkRequest request;
        Film f;
};

#endif // OMDBPARSER_H
