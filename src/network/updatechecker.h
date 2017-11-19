/*************************************************************************************************
 *                                                                                                *
 *  file: updatechecker.h                                                                         *
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

#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include "networkrequest.h"

class UpdateChecker : public QObject
{
    Q_OBJECT

    public:
        UpdateChecker();

        void Run();

    signals:
        void Loaded( const QString& latestVersion );

    private slots:
        void DataLoaded( const QByteArray& data );
        void DataLoadError( const QString& e );

    private:
        NetworkRequest request;
};

#endif // UPDATECHECKER_H
