/*************************************************************************************************
 *                                                                                                *
 *  file: statisticswindow.h                                                                      *
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

#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H


#include "filmslist/filmitem.h"
#include "tools/timecounter.h"
#include "statisticsworker.h"
#include "ui_statisticswindow.h"


#include <QAbstractItemModel>
#include <QDialog>
#include <QList>
#include <QMutex>


class StatisticsWindow : public QDialog, protected Ui::StatisticsWindow
{
    Q_OBJECT

    public:
        explicit StatisticsWindow( QWidget* parent = nullptr );

        void SetModel( QAbstractItemModel* model );

    signals:
        void ResetStatistics();

    private slots:
        void LoadStatistics( FilmItemList films );

        void IncProgress();
        void ShowMainStatistics( MainStatistics mainStat );
        void Reset();

    private:
        QAbstractItemModel* model = nullptr;
        QMutex progressMutex;
};


#endif // STATISTICSWINDOW_H

