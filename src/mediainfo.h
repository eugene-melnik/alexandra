
#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QString>

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
        MediaInfo( QString fileName );
        ~MediaInfo();

        QString GetFormat() const;
        QString GetFileSize() const;
        QString GetOverallBitRate() const;
        QString GetWidth() const;
        QString GetHeight() const;
        QString GetFrameRate() const;
        QString GetDuration() const;
        QString GetCompleteData() const;

        QString GetLibraryVersion() const;

    private:
        MI::MediaInfo* mi = nullptr;
};

#endif // MEDIAINFO_H
