/*************************************************************************************************
 *                                                                                                *
 *  file: regexptools.cpp                                                                         *
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

#include "regexptools.h"

#include <QStringList>


void RegExpTools::SimplifyText( QString& str )
{
    str = str.simplified();
    str.replace( "&nbsp;",  " " );
    str.replace( "&raquo;", "»" );
    str.replace( "&laquo;", "«" );
    str.replace( "&lt;",    "<" );
    str.replace( "&gt;",    ">" );
    str.replace( "&quot;",  "\"" );
    str.replace( "&#x27;",  "'" );
    str.replace( "%5B",     "[" );
    str.replace( "%5D",     "]" );
    str.replace( "%20",     " " );
    str.replace( "&#133;",  "…" );
    str.replace( 0x0A,      "" );
    str.replace( "> <",     "><" );
}


QString RegExpTools::ParseList( const QString& str, QRegExp& reList, QRegExp& reItem, quint16 stripTo )
{
    reItem.setMinimal( true );
    reList.setMinimal( true );
    reList.indexIn( str );

    QString list = reList.cap(1);
    QStringList result;

    int nextPosition = 0;

    while( reItem.indexIn( list, nextPosition ) != -1 )
    {
        result.append( reItem.cap(1).trimmed() );
        nextPosition += reItem.matchedLength();
    }

    result.removeDuplicates(); // FIXME: solution or not?

      // Leave only 'stripTo' first elements
    if( stripTo != 0 && stripTo < result.size() )
    {
        result.erase( result.begin() + stripTo, result.end() );
    }

    return( result.join( ", " ) );
}


QString RegExpTools::ParseItem( const QString& str, QRegExp& reItem )
{
    reItem.setMinimal( true );
    reItem.indexIn( str );
    return( reItem.cap(1).trimmed() );
}

