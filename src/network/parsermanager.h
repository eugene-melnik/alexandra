/*************************************************************************************************
 *                                                                                                *
 *  file: parsermanager.h                                                                         *
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

#ifndef PARSERMANAGER_H
#define PARSERMANAGER_H

#include <QMap>
#include <QObject>
#include <QStringList>

#include "abstractparser.h"
#include "film.h"

class ParserManager : public QObject
{
    Q_OBJECT

    public:
        enum Parser {
            Auto = 0,
            OMDB
        };

        explicit ParserManager( Parser p = Parser::Auto );

        QStringList GetAvailableParsers() const;

        void SetParserId( Parser p ) { selectedParserId = p; }
        void SetTitle( const QString& t ) { title = t; }
        void SetYear( const QString& y ) { year = y; }

        void Reset();
        void Search();
        void SearchAsync( Film* filmSaveTo, QString* posterFileNameSaveTo );

    signals:
        void Progress( quint64 received, quint64 total );
        void Loaded( const Film& f, const QString& posterFileName );
        void Error( const QString& e );

    private slots:
        void ProgressChanged( quint64 received, quint64 total ) { emit Progress( received, total ); }
        void InformationLoaded( const Film& f, const QString& posterFileName );
        void InformationLoadError( const QString& e );

        void CreateParser();

    private:
        QMap<Parser,QString> parsers;

        Parser selectedParserId;
        QString title;
        QString year;

        QObject* currentParser = nullptr;
};

#endif // PARSERMANAGER_H
