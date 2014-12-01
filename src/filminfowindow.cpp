/*************************************************************************************************
 *                                                                                                *
 *  file: filminfowindow.cpp                                                                      *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014 Eugene Melnik <jeka7js@gmail.com>                                          *
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

#include "filminfowindow.h"

#include <ZenLib/Ztring.h>

// Thanks to Mediainfo project :)
#define wstring2QString(_DATA) \
    QString::fromUtf8(ZenLib::Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    ZenLib::Ztring().From_UTF8(_DATA.toUtf8())

FilmInfoWindow::FilmInfoWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );

    mi = new MediaInfoLib::MediaInfo();
    mi->Option( __T( "CharSet" ), __T( "UTF-8" ) );
}

FilmInfoWindow::~FilmInfoWindow()
{
    delete mi;
}

void FilmInfoWindow::SetCurrentFile( const QString& fileName )
{
    mi->Open( QString2wstring( fileName ) );
    LoadShortInfo();
    mi->Close();
}

const QString &FilmInfoWindow::GetShortTechInfo() const
{
    return( shortInfo );
}

void FilmInfoWindow::LoadShortInfo()
{
    shortInfo = wstring2QString( mi->Get( MediaInfoLib::Stream_General, 0, __T( "Format" ) ).c_str() ) + " | ";
    shortInfo += wstring2QString( mi->Get( MediaInfoLib::Stream_General, 0, __T( "FileSize/String3" ) ).c_str() ) + "<br/>";
    shortInfo += wstring2QString( mi->Get( MediaInfoLib::Stream_Video, 0, __T( "Width" ) ).c_str() ) + "&times;";
    shortInfo += wstring2QString( mi->Get( MediaInfoLib::Stream_Video, 0, __T( "Height" ) ).c_str() ) + " px | ";
    shortInfo += wstring2QString( mi->Get( MediaInfoLib::Stream_Video, 0, __T( "FrameRate/String" ) ).c_str() ) + "<br/>";
    shortInfo += wstring2QString( mi->Get( MediaInfoLib::Stream_Video, 0, __T( "Duration/String" ) ).c_str() );
}
