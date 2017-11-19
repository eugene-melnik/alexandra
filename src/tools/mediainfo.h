/*************************************************************************************************
 *                                                                                                *
 *  file: mediainfo.h                                                                             *
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

#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QTime>

#ifdef Q_OS_LINUX
    #include <MediaInfo/MediaInfo.h>
    #define MI MediaInfoLib
#elif defined(Q_OS_WIN32)
    #include "MediaInfoDLL.h"
    #define MI MediaInfoDLL
#endif // MEDIAINFO_LIBRARY

class MediaInfo
{
    public:
        MediaInfo();
        MediaInfo( const QString& fileName );
        virtual ~MediaInfo();

        bool IsOpened() const { return( isOpened ); }

        QString GetFormat() const;
        QString GetFileSize() const;
        QString GetOverallBitRate() const;
        QString GetWidth() const;
        QString GetHeight() const;
        QString GetFrameRate() const;
        QString GetDuration() const;
        QTime   GetDurationTime() const;
        QString GetCompleteData() const;

        static QString GetLibraryVersion();

    private:
        MI::MediaInfo* mi = nullptr;
        bool isOpened = false;
};

#endif // MEDIAINFO_H
