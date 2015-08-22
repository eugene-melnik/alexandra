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

#include <algorithm>
#include <QMessageBox>

StatisticsWindow::StatisticsWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );

    // Worker
    statisticsWorker = new StatisticsWorker();
    connect( statisticsWorker, &StatisticsWorker::MainStatisticsLoaded, this, &StatisticsWindow::ShowMainStatistics );

    // Buttons
    connect( bReset, &QPushButton::clicked, this, &StatisticsWindow::Reset );
}

StatisticsWindow::~StatisticsWindow()
{
    delete statisticsWorker;
}

void StatisticsWindow::show( const QList<Film>* films )
{
    // Calculations
    statisticsWorker->SetFilms( films );
    statisticsWorker->start();

    // Show
    lTotalFilmsInLibrary->setText( QString::number( films->size() ) );
    progressBar->show();
    QDialog::show();
}

void StatisticsWindow::closeEvent( QCloseEvent* event )
{
    if( statisticsWorker->isRunning() )
    {
        statisticsWorker->Terminate();
    }

    // Contents
    tabWidget->setCurrentIndex( 0 ); // activate first tab
    lwMostPopularFilms->clear();
    lWastedTime->setToolTip( "" );

    event->accept();
}

void StatisticsWindow::ShowMainStatistics( int viewedFilms,
                                           int totalViewsCount,
                                           TimeCounter wastedTime,
                                           bool allFilesOk,
                                           QList<TopFilm>* topFilms )
{
    progressBar->hide();

    // Output
    lFilmsViewed->setText( QString::number( viewedFilms ) );
    lTotalViews->setText( QString::number( totalViewsCount ) );
    lWastedTime->setText( wastedTime.ToString() );

    if( !allFilesOk )
    {
        lWastedTime->setText( lWastedTime->text() + " (?)" );
        lWastedTime->setToolTip( tr( "The calculation is not accurate, because some files are not available." ) );
    }

    // Most popular
    auto TopFilmLessThen = [] ( TopFilm a, TopFilm b ) { return( a.viewsCount < b.viewsCount ); };
    std::sort( topFilms->begin(), topFilms->end(), TopFilmLessThen );

    for( int i = 0; i < topFilms->size(); i++ )
    {
        QString itemText = QString( "(%1) " ).arg( topFilms->at(i).viewsCount ) + topFilms->at(i).filmTitle;
        lwMostPopularFilms->insertItem( 0, itemText );
    }

    delete topFilms;
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