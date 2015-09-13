/*************************************************************************************************
 *                                                                                                *
 *  file: timecounter.h                                                                           *
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

#ifndef TIMECOUNTER_H
#define TIMECOUNTER_H

#include <QTime>

class TimeCounter
{
    public:
        TimeCounter() { Reset(); }
        TimeCounter( quint64 s );
        TimeCounter( quint16 h, quint16 m, quint16 s, quint16 ms = 0 );
        TimeCounter( quint16 d, quint16 h, quint16 m, quint16 s, quint16 ms = 0 );
        TimeCounter( const QTime& t );
        TimeCounter( const TimeCounter& t ) = default;

        void Add( const TimeCounter& t );
        void Add( const QTime& t );

        void Reset();

        QString ToString() const;
        QString ToStringWithDays() const;
        QString ToStringWithMs() const;

    private:
        quint32 days;
        quint16 hours;
        quint16 minutes;
        quint16 seconds;
        quint16 mseconds;
};

#endif // TIMECOUNTER_H
