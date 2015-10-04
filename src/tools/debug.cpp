/*************************************************************************************************
 *                                                                                                *
 *  file: debug.cpp                                                                               *
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

#include "debug.h"

#ifdef QT_DEBUG

#include <cstdio>
#include <QTime>

const char* timeFormat = "hh:mm:ss.zzz";

void DebugPrint( const QString& text )
{
    printf( "[%s] %s\n", QTime::currentTime().toString( timeFormat ).toStdString().c_str(),
                         text.toStdString().c_str() );
}

void DebugPrintL( const QString& text )
{
    printf( "\n[%s] %s\n\n", QTime::currentTime().toString( timeFormat ).toStdString().c_str(),
                             text.toStdString().c_str() );
}

void DebugPrintFunc( const char* funcName )
{
    printf( "\n[%s] %s()\n", QTime::currentTime().toString( timeFormat ).toStdString().c_str(),
                             funcName );
}

void DebugPrintFuncA( const char* funcName, int argument )
{
    printf( "\n[%s] %s( %d )\n", QTime::currentTime().toString( timeFormat ).toStdString().c_str(),
                                 funcName,
                                 argument );
}

void DebugPrintFuncA( const char* funcName, const QString& argument )
{
    printf( "\n[%s] %s( %s )\n", QTime::currentTime().toString( timeFormat ).toStdString().c_str(),
                                 funcName,
                                 argument.toStdString().c_str() );
}


void DebugPrintFuncDone( const char* funcName )
{
    printf( "[%s] Done <%s>\n", QTime::currentTime().toString( timeFormat ).toStdString().c_str(),
                                funcName );
}

#endif // QT_DEBUG
