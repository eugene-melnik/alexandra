/*************************************************************************************************
 *                                                                                                *
 *  file: statisticswindow.cpp                                                                    *
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

#include "statisticswindow.h"
#include "tools/debug.h"

#include <algorithm>
#include <QMessageBox>
#include <QMutexLocker>


StatisticsWindow::StatisticsWindow( QWidget* parent ) : QDialog( parent )
{
    DebugPrint( "StatisticsWindow::StatisticsWindow" );

    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );
    connect( bReset, &QPushButton::clicked, this, &StatisticsWindow::Reset );
}


void StatisticsWindow::SetModel( QAbstractItemModel* model )
{
    this->model = model;
    FilmItemList films;

    for( int row = 0; row < model->rowCount(); ++row )
    {
        FilmItem* film = static_cast<FilmItem*>( model->index( row, 0 ).internalPointer() );
        films.append( film );
    }

    lTotalFilmsInLibrary->setText( QString::number(films.size()) );
    progressBar->setMaximum( films.size() );
    progressBar->setValue( 0 );

    LoadStatistics( films );
}


void StatisticsWindow::LoadStatistics( FilmItemList films )
{
    StatisticsWorker* calcWorker = new StatisticsWorker();
    connect( this, SIGNAL(destroyed()), calcWorker, SLOT(Terminate()) ); // On window close while calculating
    connect( calcWorker, &StatisticsWorker::IncProgress,          this, &StatisticsWindow::IncProgress );
    connect( calcWorker, &StatisticsWorker::MainStatisticsLoaded, this, &StatisticsWindow::ShowMainStatistics );
    connect( calcWorker, &StatisticsWorker::finished,             calcWorker, &QWidget::deleteLater );
    calcWorker->SetFilms( films );
    calcWorker->start();
}


void StatisticsWindow::IncProgress()
{
    QMutexLocker locker( &progressMutex );
    progressBar->setValue( progressBar->value() + 1 );
    progressBar->repaint();
}


void StatisticsWindow::ShowMainStatistics( MainStatistics mainStat )
{
    lFavouriteFilms->setText( QString::number( mainStat.favouriteFilms ) );
    double ratio = mainStat.viewedFilms * 100.0 / lTotalFilmsInLibrary->text().toInt();
    lFilmsViewed->setText( QString( "%1 (%L2%)" ).arg( mainStat.viewedFilms ).arg( ratio, 0, 'f', 1 ) );
    lTotalViews->setText( QString::number(mainStat.totalViewsCount) );
    lWastedTime->setText( mainStat.wastedTime.ToString() );

    #ifdef MEDIAINFO_SUPPORT
        if( !mainStat.allFilesOk ) // Condition when unable to access all the files
        {
            lWastedTime->setText( lWastedTime->text() + " (?)" );
            lWastedTime->setToolTip( tr("The calculations are inaccurate because some of files aren't available.") );
        }
    #else
        labelWastedTime->hide();
        lWastedTime->hide();
    #endif // MEDIAINFO_SUPPORT

      // List of most popular films
      // Sorting by views count and alphabet
    std::sort( mainStat.topFilms.begin(), mainStat.topFilms.end(), [] (TopFilm a, TopFilm b)
    {
        if( a.viewsCount == b.viewsCount )
        {
            return( a.filmTitle > b.filmTitle );
        }
        else
        {
            return( a.viewsCount < b.viewsCount );
        }
    } );

      // Numbers of
    lNumberOfDirectors->setText( QString::number(mainStat.directorsCount) );
    lNumberOfScreenwriters->setText( QString::number(mainStat.screenwritersCount) );
    lNumberOfGenres->setText( QString::number(mainStat.genresCount) );
    lNumberOfActors->setText( QString::number(mainStat.actorsCount) );
    lNumberOfCountries->setText( QString::number(mainStat.countriesCount) );

      // Show in list widget
    for( int i = 0; i < mainStat.topFilms.size(); ++i )
    {
        QString itemText = QString( "(%1) " ).arg( mainStat.topFilms.at(i).viewsCount ) + mainStat.topFilms.at(i).filmTitle;
        lwMostPopularFilms->insertItem( 0, itemText );
    }

    progressBar->hide();
}


void StatisticsWindow::Reset()
{
    int answer = QMessageBox::warning( this, tr("Reset statistics"), tr("Are you sure?"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

    if( answer == QMessageBox::Yes )
    {
        emit ResetStatistics();
    }
}

