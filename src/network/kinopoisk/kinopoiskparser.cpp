/*************************************************************************************************
 *                                                                                                *
 *  file: kinopoiskparser.cpp                                                                     *
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

#include "kinopoiskparser.h"
#include "debug.h"

#include <QDir>
#include <QFile>
#include <QTextCodec>

void KinopoiskParser::SearchFor( const QString& title, const QString& year )
{
    searchUrlWithYear = QString( "http://www.kinopoisk.ru/index.php?level=7&m_act[what]=content&first=yes&m_act[find]=%1&m_act[year]=%2" );
    searchUrl = QString( "http://www.kinopoisk.ru/index.php?first=yes&what=&kp_query=%1" );
    AbstractParser::SearchFor( title, year );
}

void KinopoiskParser::SyncSearchFor( Film * filmSaveTo, QString* posterFileNameSaveTo,
                                     const QString& title, const QString& year )
{
    searchUrlWithYear = QString( "http://www.kinopoisk.ru/index.php?level=7&m_act[what]=content&first=yes&m_act[find]=%1&m_act[year]=%2" );
    searchUrl = QString( "http://www.kinopoisk.ru/index.php?first=yes&what=&kp_query=%1" );
    AbstractParser::SyncSearchFor( filmSaveTo, posterFileNameSaveTo, title, year );
}

QString KinopoiskParser::Parse( const QByteArray& data )
{
    DebugPrintFuncA( "KinopoiskParser::Parse", data.size() );

    QTextCodec* codec = QTextCodec::codecForName( "Windows-1251" );
    QString str = codec->toUnicode( data );

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

    DebugPrint( "Simpified to: " + QString::number( str.size() ) );

//QFile file("../INDEX.HTML");
//file.open(QIODevice::WriteOnly);
//file.write(str.toUtf8());
//file.close();

    // Title
    QRegExp reTitle( "class=\"moviename-big\".*>(.*)</h1>" );
    reTitle.setMinimal( true );
    reTitle.indexIn( str );
    f.SetTitle( reTitle.cap(1) );

    // Original title
    QRegExp reOriginalTitle( "itemprop=\"alternativeHeadline\">(.*)</span>" );
    reOriginalTitle.setMinimal( true );
    reOriginalTitle.indexIn( str );
    f.SetOriginalTitle( reOriginalTitle.cap(1) );

    // Tagline
    QRegExp reTagline( "слоган</td><.*>(.*)</td>" );
    reTagline.setMinimal( true );
    reTagline.indexIn( str );
    f.SetTagline( reTagline.cap(1) );

    // Year
    QRegExp reYear( "год</td><td><.*><.*>(.*)</a>" );
    reYear.setMinimal( true );
    reYear.indexIn( str );
    f.SetYearFromStr( reYear.cap(1).trimmed() );

    // Country
    QRegExp reCountry( "страна</td><td><.*><.*>(.*)</a>" );
    reCountry.setMinimal( true );
    reCountry.indexIn( str );
    f.SetCountry( reCountry.cap(1) );

    // Director /// может быть список людей
    QRegExp reDirector( "режиссер</td><.*><.*>(.*)</a>" );
    reDirector.setMinimal( true );
    reDirector.indexIn( str );
    f.SetDirector( reDirector.cap(1) );

    // Starring
    QRegExp reStarring( "В главных ролях:</h4>(.*)</ul>" );
    reStarring.setMinimal( true );
    reStarring.indexIn( str );

    QRegExp reActors( "href=\"/name/.*>(.*)</a>" );
    reActors.setMinimal( true );

    QStringList actors;
    int pos = 0;

    while( reActors.indexIn( reStarring.cap(1), pos ) != -1 )
    {
        actors.append( reActors.cap(1) );
        pos += reActors.matchedLength();
    }

    f.SetStarring( actors.join( ", " ) );

    // Description
    QRegExp reDescription( "itemprop=\"description\">(.*)</div>" );
    reDescription.setMinimal( true );
    reDescription.indexIn( str );
    f.SetDescription( reDescription.cap(1).replace( "<br><br>", "<br>\n" ) );

    DebugPrint( "Seems to be done!" );
    emit Loaded( f, QString() );
    return( QString() );
}
