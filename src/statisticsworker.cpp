/*************************************************************************************************
 *                                                                                                *
 *  file: statisticsworker.cpp                                                                    *
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

#include "mediainfo.h"
#include "statisticsworker.h"

void StatisticsWorker::run()
{
    isTerminate = false;

    // Calculations
    int viewedFilms = 0;
    int totalViewsCount = 0;
    TimeCounter wastedTime;
    bool allFilesOk = true;

    QList<TopFilm>* topFilms = new QList<TopFilm>();

    for( QList<Film>::const_iterator i = films->begin(); i < films->end(); i++ )
    {
        if( isTerminate ) return; // statistics window closed

        if( i->GetIsViewed() )
        {
            viewedFilms++;

            if( i->GetViewsCounter() != 0 )
            {
                totalViewsCount += i->GetViewsCounter();

                // Wasted time
                MediaInfo f( i->GetFileName() );

                if( f.IsOpened() )
                {
                    TimeCounter duration( f.GetDurationTime() );

                    for( int j = i->GetViewsCounter(); j > 0; j-- )
                    {
                        wastedTime.Add( duration );
                    }
                }
                else
                {
                    allFilesOk = false;
                }

                // Most popular
                topFilms->append( { i->GetViewsCounter(), i->GetTitle() } );
            }
        }
    }

    emit MainStatisticsLoaded( viewedFilms, totalViewsCount, wastedTime, allFilesOk, topFilms );
}
