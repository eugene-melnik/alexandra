/*************************************************************************************************
 *                                                                                                *
 *  file: timecounter.cpp                                                                         *
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

#include "timecounter.h"

TimeCounter::TimeCounter()
{
    days = 0;
    hours = 0;
    minutes = 0;
    seconds = 0;
    mseconds = 0;
}

TimeCounter::TimeCounter( quint16 h, quint16 m, quint16 s, quint16 ms )
{
    days = 0;
    hours = h;
    minutes = m;
    seconds = s;
    mseconds = ms;
}

TimeCounter::TimeCounter( quint16 d, quint16 h, quint16 m, quint16 s, quint16 ms )
{
    days = d;
    hours = h;
    minutes = m;
    seconds = s;
    mseconds = ms;
}

TimeCounter::TimeCounter( const QTime& t )
{
    days = 0;
    hours = t.hour();
    minutes = t.minute();
    seconds = t.second();
    mseconds = t.msec();
}

TimeCounter::TimeCounter( const TimeCounter& t )
{
    days = t.days;
    hours = t.hours;
    minutes = t.minutes;
    seconds = t.seconds;
    mseconds = t.mseconds;
}

void TimeCounter::Add( const TimeCounter& t )
{
    if( (mseconds += t.mseconds) >= 1000 )
    {
        mseconds -= 1000;
        seconds++;
    }

    if( (seconds += t.seconds) >= 60 )
    {
        seconds -= 60;
        minutes++;
    }

    if( (minutes += t.minutes) >= 60 )
    {
        minutes -= 60;
        hours++;
    }

    if( (hours += t.hours) >= 24 )
    {
        hours -= 24;
        days++;
    }
}

void TimeCounter::Add( const QTime& t )
{
    Add( TimeCounter( t ) );
}

QString TimeCounter::ToString()
{
    QString result = QString( "%1/%2:%3:%4" )
            .arg( days )
            .arg( hours )
            .arg( minutes )
            .arg( seconds );

    return( result );
}

QString TimeCounter::ToStringWithMs()
{
    QString result = QString( "%1/%2:%3:%4.%5" )
            .arg( days )
            .arg( hours )
            .arg( minutes )
            .arg( seconds )
            .arg( mseconds );

    return( result );
}
