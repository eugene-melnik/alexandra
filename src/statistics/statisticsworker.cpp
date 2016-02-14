/*************************************************************************************************
 *                                                                                                *
 *  file: statisticsworker.cpp                                                                    *
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

#include "statisticsworker.h"
#include "tools/mediainfo.h"


StatisticsWorker::StatisticsWorker() : QThread()
{
    qRegisterMetaType<TimeCounter>( "TimeCounter" );
    qRegisterMetaType<TopFilmList>( "TopFilmList" );
}


void StatisticsWorker::run()
{
    int viewedFilms = 0;
    int totalViewsCount = 0;
    bool allFilesOk = true;

    TimeCounter wastedTime;
    QList<TopFilm> topFilms;

      // Calculations
    for( FilmItem* film : films )
    {
        if( isTerminate ) return; // statistics window closed

        if( film->GetIsFilmViewed() )
        {
            viewedFilms++;
            int viewsCount = film->GetColumnData( FilmItem::ViewsCountColumn ).toInt();

            if( viewsCount != 0 )
            {
                totalViewsCount += viewsCount;

                  // Wasted time
                #ifdef MEDIAINFO_SUPPORT
                    MediaInfo mi( film->GetFileName() );

                    if( mi.IsOpened() )
                    {
                        TimeCounter duration( mi.GetDurationTime() );
                        wastedTime = duration;
                        wastedTime *= viewsCount;
                    }
                    else
                    {
                        allFilesOk = false;
                    }
                #endif // MEDIAINFO_SUPPORT

                  // Most popular
                topFilms.append( { film->GetTitle(), viewsCount } );
            }
        }

        emit IncProgress();
    }

    emit MainStatisticsLoaded( viewedFilms, totalViewsCount, wastedTime, allFilesOk, topFilms );
}

