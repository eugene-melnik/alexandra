/*************************************************************************************************
 *                                                                                                *
 *  file: filmscanneraddworker.h                                                                  *
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

#ifndef FILMSCANNERADDWORKER_H
#define FILMSCANNERADDWORKER_H


#include "alexandrasettings.h"
#include "filmslist/filmitem.h"
#include "parsers/parsermanager.h"


#include <QList>
#include <QMetaType>
#include <QStringList>
#include <QTableWidget>
#include <QThread>


class FilmScannerAddWorker : public QThread
{
    Q_OBJECT

    public:
        FilmScannerAddWorker();

        void SetFoundedFilms( QStringList founded ) { foundedFilms = founded; }
        void SetLoadInformation( bool load ) { loadInformation = load; }
        void SetSearchForPoster( bool load ) { searchForPoster = load; }

        void Cancel() { isCanceled = true; parser.Abort(); }

    signals:
        void FilmCreated( FilmItem* );

    protected:
        void run() override;

    private:
        bool SavePosterTo( QString sourceName, QString destinationName );

        ParserManager parser;
        QStringList foundedFilms;
        bool loadInformation = false;
        bool searchForPoster = false;
        bool isCanceled = false;

        AlexandraSettings* settings;
};


#endif // FILMSCANNERADDWORKER_H

