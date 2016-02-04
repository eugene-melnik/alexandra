/*************************************************************************************************
 *                                                                                                *
 *  file: debug.h                                                                                 *
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

#ifndef DEBUG_H
#define DEBUG_H

#include <QApplication>

#ifdef QT_DEBUG

/*
 * Output format:
 *   [time] text
 */

    void DebugPrint( const QString& text );
    void DebugPrintL( const QString& text );

/*
 * Output format:
 *   [time] FunctionName()
 *   [time] FunctionName( argument )
 *   [time] FunctionName( arg, um, ents )
 *
 * For example:
 *   [21:21:01.123] AlexandraSettings::AlexandraSettings( configuration.conf )
 */

    void DebugPrintFunc( const char* funcName );
    void DebugPrintFuncA( const char* funcName, int argument );
    void DebugPrintFuncA( const char* funcName, const QString& argument );

/*
 * Output format:
 *   [time] Done <FunctionName> [<ElapsedTime> ms]
 */

    void DebugPrintFuncDone( const char* funcName );

#else

    #define DebugPrint(text)
    #define DebugPrintL(text)

    #define DebugPrintFunc(...)
///    #define DebugPrintFuncA(funcName, argument)

    #define DebugPrintFuncDone(funcName)

#endif

#endif //DEBUG_H
