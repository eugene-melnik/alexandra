/*************************************************************************************************
 *                                                                                                *
 *  file: statisticsworker.h                                                                      *
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

#ifndef STATISTICSWORKER_H
#define STATISTICSWORKER_H

#include <QList>
#include <QThread>

#include "film.h"
#include "timecounter.h"

typedef struct {
    int viewsCount;
    QString filmTitle;
} TopFilm;

class StatisticsWorker : public QThread
{
    Q_OBJECT

    public:
        StatisticsWorker();

        void SetFilms( const QList<Film>* f ) { films = f; }
        void Terminate() { isTerminate = true; }

    signals:
        void MainStatisticsLoaded( int              /* viewedFilms */,
                                   int              /* totalViewsCount */,
                                   TimeCounter      /* wastedTime */,
                                   bool             /* allFilesOk */,
                                   QList<TopFilm>*  /* topFilms */ );

    protected:
        void run() override;

    private:
        const QList<Film>* films;
        bool isTerminate = false;
};

#endif // STATISTICSWORKER_H
