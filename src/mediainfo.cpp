/*************************************************************************************************
 *                                                                                                *
 *  file: mediainfo.cpp                                                                           *
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

#include "mediainfo.h"

#include <QStringList>

MediaInfo::MediaInfo()
{
    mi = new MI::MediaInfo();
    mi->Option( __T( "CharSet" ), __T( "UTF-8" ) );
}

MediaInfo::MediaInfo( QString fileName )
{
    mi = new MI::MediaInfo();
    mi->Option( __T( "CharSet" ), __T( "UTF-8" ) );

    if( mi->Open( MI::String( fileName.toStdWString() ) ) == 1 )
    {
        isOpened = true;
    }
}

MediaInfo::~MediaInfo()
{
    mi->Close();
    delete mi;
}

bool MediaInfo::IsOpened() const
{
    return( isOpened );
}

QString MediaInfo::GetFormat() const
{
    return( QString::fromStdWString( mi->Get( MI::Stream_General, 0, __T( "Format" ) ) ) );
}

QString MediaInfo::GetFileSize() const
{
    return( QString::fromStdWString( mi->Get( MI::Stream_General, 0, __T( "FileSize/String3" ) ) ) );
}

QString MediaInfo::GetOverallBitRate() const
{
    return( QString::fromStdWString( mi->Get( MI::Stream_General, 0, __T( "OverallBitRate/String" ) ) ) );
}

QString MediaInfo::GetWidth() const
{
    return( QString::fromStdWString( mi->Get( MI::Stream_Video, 0, __T( "Width" ) ) ) );
}

QString MediaInfo::GetHeight() const
{
    return( QString::fromStdWString( mi->Get( MI::Stream_Video, 0, __T( "Height" ) ) ) );
}

QString MediaInfo::GetFrameRate() const
{
    return( QString::fromStdWString( mi->Get( MI::Stream_Video, 0, __T( "FrameRate" ) ) ) );
}

QString MediaInfo::GetDuration() const
{
    return( QString::fromStdWString( mi->Get( MI::Stream_General, 0, __T( "Duration/String3" ) ) ).split(".").at(0) ); // without mseconds
}

QTime MediaInfo::GetDurationTime() const
{
    QString timeStr = QString::fromStdWString( mi->Get( MI::Stream_General, 0, __T( "Duration/String3" ) ) );
    return( QTime::fromString( timeStr, "hh:mm:ss.zzz" ) );
}

QString MediaInfo::GetCompleteData() const
{
    mi->Option( __T( "Complete" ) );
    return( QString::fromStdWString( mi->Inform() ) );
}

QString MediaInfo::GetLibraryVersion()
{
    MI::MediaInfo mi;
    return( QString::fromStdWString( mi.Option( __T( "Info_Version" ) ) ) );
}
