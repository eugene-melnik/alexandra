/*************************************************************************************************
 *                                                                                                *
 *  file: statisticswindow.h                                                                      *
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

#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include "film.h"
#include "statisticsworker.h"
#include "timecounter.h"
#include "ui_statisticswindow.h"

#include <QCloseEvent>
#include <QDialog>
#include <QList>

class StatisticsWindow : public QDialog, public Ui::StatisticsWindow
{
    Q_OBJECT

    public:
        StatisticsWindow( QWidget* parent = nullptr );
        ~StatisticsWindow();

        void show( const QList<Film>* films );

    signals:
        void ResetStatistics();

    protected:
        void reject() { close(); }
        void closeEvent( QCloseEvent* event );

    private slots:
        void ShowMainStatistics( int viewedFilms,
                                 int totalViewsCount,
                                 TimeCounter wastedTime,
                                 bool allFilesOk,
                                 QList<TopFilm>* topFilms );

        void Reset();

    private:
        StatisticsWorker* statisticsWorker = nullptr;
};

#endif // STATISTICSWINDOW_H