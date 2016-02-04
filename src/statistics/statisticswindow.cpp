/*************************************************************************************************
 *                                                                                                *
 *  file: statisticswindow.cpp                                                                    *
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

#include "statisticswindow.h"
#include "tools/debug.h"

#include <algorithm>
#include <QMessageBox>

StatisticsWindow::StatisticsWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );
    connect( bReset, &QPushButton::clicked, this, &StatisticsWindow::Reset );
}

StatisticsWindow::~StatisticsWindow()
{
    if( calculateMutex.tryLock( 5000 ) )
    {
        calculateMutex.unlock();
    }
}

void StatisticsWindow::show( const QList<Film>* films )
{
    DebugPrintFuncA( "StatisticsWindow::show", films->size() );

    viewedFilms = 0;
    totalViewsCount = 0;
    wastedTime.Reset();
    allFilesOk = true;

    // Calculations in multithread
    threadsCount = QThread::idealThreadCount();

    if( threadsCount < 1 )
    {
        threadsCount = 1;
    }

    int subListLength = films->size() / threadsCount;

    for( int threadNum = 0; threadNum < threadsCount; threadNum++ )
    {
        int subListPos = threadNum * subListLength;

        if( threadNum == (threadsCount - 1) ) // Last thread
        {
            subListLength = -1;
        }

        StatisticsWorker* calcWorker = new StatisticsWorker();
        connect( calcWorker, &StatisticsWorker::IncProgress, this, &StatisticsWindow::IncProgress );
        connect( calcWorker, &StatisticsWorker::MainStatisticsLoaded, this, &StatisticsWindow::ShowMainStatistics );
        connect( calcWorker, &StatisticsWorker::finished, calcWorker, &QWidget::deleteLater );

        calcWorker->SetFilms( films->mid( subListPos, subListLength ) );
        calcWorker->start();
    }

    // Show
    tabWidget->setCurrentIndex( 0 ); // Activate first tab
    lTotalFilmsInLibrary->setText( QString::number( films->size() ) );
    lWastedTime->setToolTip( "" );

    progressBar->setMaximum( films->size() );
    progressBar->setValue( 0 );
    progressBar->show();

    QDialog::show();
}

void StatisticsWindow::IncProgress()
{
     progressMutex.lock();
     progressBar->setValue( progressBar->value() + 1 );
     progressBar->repaint();
     progressMutex.unlock();
}

void StatisticsWindow::ShowMainStatistics( int          threadViewedFilms,
                                           int          threadTotalViewsCount,
                                           TimeCounter  threadWastedTime,
                                           bool         threadAllFilesOk,
                                           TopFilmList* threadTopFilms )
{
    calculateMutex.lock();

    // Appending data
    viewedFilms += threadViewedFilms;
    totalViewsCount += threadTotalViewsCount;
    wastedTime.Add( threadWastedTime );
    allFilesOk |= threadAllFilesOk;

    topFilms.append( *threadTopFilms );
    delete threadTopFilms;

    // Other information
    lFilmsViewed->setText( QString::number( viewedFilms ) );
    lTotalViews->setText( QString::number( totalViewsCount ) );
    lWastedTime->setText( wastedTime.ToString() );

    if( --threadsCount == 0 ) // End of scanning
    {
#ifdef MEDIAINFO_SUPPORT
        if( !allFilesOk ) // Condition when unable to access all the files
        {
            lWastedTime->setText( lWastedTime->text() + " (?)" );
            lWastedTime->setToolTip( tr( "The calculation is not accurate, because some files are not available." ) );
        }
#else
        labelWastedTime->hide();
        lWastedTime->hide();
#endif

        // List of most popular films
        // Sorting by views count and alphabet
        std::sort( topFilms.begin(), topFilms.end(), [] ( TopFilm a, TopFilm b )
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

        // Show in list widget
        for( int i = 0; i < topFilms.size(); i++ )
        {
            QString itemText = QString( "(%1) " ).arg( topFilms.at(i).viewsCount ) + topFilms.at(i).filmTitle;
            lwMostPopularFilms->insertItem( 0, itemText );
        }

        progressBar->hide();
        topFilms.clear();
    }

    calculateMutex.unlock();
}

void StatisticsWindow::Reset()
{
    int res = QMessageBox::warning( this,
                                    tr( "Reset statistics" ),
                                    tr( "Are you sure?" ),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

    if( res == QMessageBox::Yes )
    {
        emit ResetStatistics();
    }

    close();
}
