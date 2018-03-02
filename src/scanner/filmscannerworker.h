/*************************************************************************************************
 *                                                                                                *
 *  file: filmscannerworker.h                                                                     *
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

#ifndef FILMSCANNERWORKER_H
#define FILMSCANNERWORKER_H


#include <QStringList>
#include <QThread>


class FilmScannerWorker : public QThread
{
    Q_OBJECT

    public:
        FilmScannerWorker() = default;

        void SetIsRecursive( bool b ) { isRecursive = b; }
        void SetDir( const QString& d ) { selectedDir = d; }

        void Cancel() { isCanceled = true; }
        void Terminate() { isTerminated = true; isCanceled = true; }

    signals:
        void IncFoundTotal();
        void Scanned( QStringList );

    protected:
        void run() override;

    private:
        QStringList ScanDirectory( const QString& dir );
        QStringList ScanDirectoryRecursive( const QString& dir );

        QString selectedDir;
        bool isRecursive = false;
        bool isCanceled;
        bool isTerminated;
};


#endif // FILMSCANNERWORKER_H
