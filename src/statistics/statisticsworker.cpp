/*************************************************************************************************
 *                                                                                                *
 *  file: statisticsworker.cpp                                                                    *
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

#include "statisticsworker.h"
#include "tools/mediainfo.h"


StatisticsWorker::StatisticsWorker() : QThread()
{
    qRegisterMetaType<MainStatistics>( "MainStatistics" );
}


void StatisticsWorker::run()
{
    MainStatistics mainStat = MainStatistics();
    QStringList directors, screenwriters, genres, actors, countries;

      // Viewed
    for( FilmItem* film : films )
    {
        if( isTerminate ) return; // Statistics window closed

        if( film->GetIsFilmViewed() )
        {
            mainStat.viewedFilms++;
            int viewsCount = film->GetColumnData( FilmItem::ViewsCountColumn ).toInt();

            if( viewsCount != 0 )
            {
                mainStat.totalViewsCount += viewsCount;

                  // Wasted time
                #ifdef MEDIAINFO_SUPPORT
                    MediaInfo mi( film->GetFileName() );

                    if( mi.IsOpened() )
                    {
                        TimeCounter duration( mi.GetDurationTime() );
                        duration *= viewsCount;
                        mainStat.wastedTime += duration;
                    }
                    else
                    {
                        mainStat.allFilesOk = false;
                    }
                #endif // MEDIAINFO_SUPPORT

                  // Most popular
                TopFilm topFilm = { film->GetTitle(), viewsCount };
                mainStat.topFilms.append( topFilm );
            }
        }

          // Favourite
        if( film->GetIsFilmFavourite() )
        {
            mainStat.favouriteFilms++;
        }

          // Directors
        QString filmDirectors = film->GetColumnData( FilmItem::DirectorColumn ).toString();
        directors.append( StringToList(filmDirectors) );
        directors.removeDuplicates();

          // Screenwriters
        QString filmScreenwriters = film->GetColumnData( FilmItem::ScreenwriterColumn ).toString();
        screenwriters.append( StringToList(filmScreenwriters) );
        screenwriters.removeDuplicates();

          // Genres
        QString filmGenres = film->GetColumnData( FilmItem::GenreColumn ).toString();
        genres.append( StringToList(filmGenres) );
        genres.removeDuplicates();

          // Actors
        QString filmStarring = film->GetColumnData( FilmItem::StarringColumn ).toString();
        actors.append( StringToList(filmStarring) );
        actors.removeDuplicates();

          // Countries
        QString filmCountries = film->GetColumnData( FilmItem::CountryColumn ).toString();
        countries.append( StringToList(filmCountries) );
        countries.removeDuplicates();

        emit IncProgress();
    }

    mainStat.directorsCount = directors.count();
    mainStat.screenwritersCount = screenwriters.count();
    mainStat.genresCount = genres.count();
    mainStat.actorsCount = actors.count();
    mainStat.countriesCount = countries.count();

    emit MainStatisticsLoaded( mainStat );
}


QStringList StatisticsWorker::StringToList( const QString& str )
{
    if( str.isEmpty() )
    {
        return( QStringList() );
    }
    else
    {
        QStringList strings = str.split( "," );

        for( QString& s : strings )
        {
            s = s.trimmed();
        }

        return( strings );
    }
}

