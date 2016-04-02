/**************************1***********************************************************************
 *                                                                                                *
 *  file: parsermanager.h                                                                         *
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

#ifndef PARSERMANAGER_H
#define PARSERMANAGER_H


#include <QDir>
#include <QMap>
#include <QStringList>
#include <QTime>
#include <QUrl>


#include "filmslist/filmitem.h"


class ParserManager : public QObject
{
    Q_OBJECT

    public:
        enum Parser
        {
            Auto = 0,
            IMDB,
            Kinopoisk,
            Kinoteatr,
            OMDB
        };

        explicit ParserManager( Parser p = Parser::Auto );
        QStringList GetAvailableParsers() const { return( parsers.values() ); }

        void SetParserId( Parser p ) { selectedParserId = p; }
        void SetTitle( const QString& t ) { title = t; }
        void SetYear( const QString& y ) { year = y; }
        void SetYear( int y ) { year = QString::number(y); }
        void SetLoadPoster( bool b ) { loadPoster = b; }

        void Reset();
        void Search();
        void SearchSync( FilmItem* filmSaveTo, QString* posterFileNameSaveTo );

    signals:
        void Progress( quint64 received, quint64 total );
        void Loaded( const FilmItem& film, const QString& posterFileName );
        void Error( const QString& );

    private slots:
        void ProgressChanged( quint64 received, quint64 total ) { emit Progress( received, total ); }
        void InformationLoaded( FilmItem film, const QUrl& posterUrl );
        void InformationLoadError( const QString& errorString );

        void CreateParser();

        bool SavePoster( QUrl posterUrl, QString posterFileName );

    private:
        QMap<Parser,QString> parsers;

        Parser selectedParserId;
        QString title;
        QString year;
        bool loadPoster;

        QObject* currentParser = nullptr;

        const QString stdPosterFileName = QDir::tempPath() + QString( "/tmpPoster%1" ).arg( rand() );

          // For debug
        QTime time;
};


#endif // PARSERMANAGER_H

