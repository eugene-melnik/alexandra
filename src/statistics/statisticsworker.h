/*************************************************************************************************
 *                                                                                                *
 *  file: statisticsworker.h                                                                      *
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

#ifndef STATISTICSWORKER_H
#define STATISTICSWORKER_H


#include <QList>
#include <QMetaType>
#include <QThread>


#include "filmslist/filmitem.h"
#include "tools/timecounter.h"


struct TopFilm
{
    QString  filmTitle;
    int      viewsCount;
};

typedef QList<TopFilm> TopFilmList;


struct MainStatistics
{
    int favouriteFilms;
    int viewedFilms;
    int totalViewsCount;
    bool allFilesOk = true;
    TimeCounter wastedTime;

    int directorsCount;
    int screenwritersCount;
    int genresCount;
    int actorsCount;
    int countriesCount;

    TopFilmList topFilms;
};


class StatisticsWorker : public QThread
{
    Q_OBJECT

    public:
        StatisticsWorker();

    public slots:
        void SetFilms( FilmItemList films ) { this->films = films; }
        void Terminate() { isTerminate = true; }

    signals:
        void IncProgress();

        void MainStatisticsLoaded( MainStatistics );

    protected:
        void run() override;

    private:
        QStringList StringToList( const QString& str );

        FilmItemList films;
        bool isTerminate = false;
};


#endif // STATISTICSWORKER_H

