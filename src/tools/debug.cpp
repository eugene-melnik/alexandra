/*************************************************************************************************
 *                                                                                                *
 *  file: debug.cpp                                                                               *
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

#include "debug.h"

#ifdef QT_DEBUG

#include <cstdio>
#include <QTime>
#include <QMap>

const char* timeFormat = "hh:mm:ss.zzz";

QMap<QString, QTime> funcTime;


void DebugPrint( const QString& text )
{
    printf( "[%s] %s\n", QTime::currentTime().toString( timeFormat ).toUtf8().data(),
                         text.toUtf8().data() );
}


void DebugPrintL( const QString& text )
{
    printf( "\n[%s] %s\n\n", QTime::currentTime().toString( timeFormat ).toUtf8().data(),
                             text.toUtf8().data() );
}


void DebugPrintFunc( const char* funcName )
{
    QTime a;
    a.start();
    funcTime.insert( QString(funcName), a );

    printf( "\n[%s] %s()\n", QTime::currentTime().toString( timeFormat ).toUtf8().data(),
                             funcName );
}


void DebugPrintFuncA( const char* funcName, int argument )
{
    QTime a;
    a.start();
    funcTime.insert( QString(funcName), a );

    printf( "\n[%s] %s( %d )\n", QTime::currentTime().toString( timeFormat ).toUtf8().data(),
                                 funcName,
                                 argument );
}


void DebugPrintFuncA( const char* funcName, const QString& argument )
{
    QTime a;
    a.start();
    funcTime.insert( QString(funcName), a );

    printf( "\n[%s] %s( %s )\n", QTime::currentTime().toString( timeFormat ).toUtf8().data(),
                                 funcName,
                                 argument.toUtf8().data() );
}


void DebugPrintFuncDone( const char* funcName )
{
    printf( "[%s] Done <%s> [%i ms]\n", QTime::currentTime().toString( timeFormat ).toUtf8().data(),
                                        funcName,
                                        funcTime.value( QString(funcName) ).elapsed() );

    funcTime.remove( QString(funcName) );
}

#endif // QT_DEBUG
