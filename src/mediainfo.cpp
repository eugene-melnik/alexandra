
#include "mediainfo.h"

MediaInfo::MediaInfo()
{
    mi = new MI::MediaInfo();
    mi->Option( __T( "CharSet" ), __T( "UTF-8" ) );
}

MediaInfo::MediaInfo( QString fileName )
{
    mi = new MI::MediaInfo();
    mi->Option( __T( "CharSet" ), __T( "UTF-8" ) );
    mi->Open( MI::String( fileName.toStdWString() ) );
}

MediaInfo::~MediaInfo()
{
    mi->Close();
    delete mi;
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
    return( QString::fromStdWString( mi->Get( MI::Stream_Video, 0, __T( "FrameRate/String" ) ) ) );
}

QString MediaInfo::GetDuration() const
{
    return( QString::fromStdWString( mi->Get( MI::Stream_General, 0, __T( "Duration/String" ) ) ) );
}

QString MediaInfo::GetCompleteData() const
{
    mi->Option( __T( "Complete" ) );
    return( QString::fromStdWString( mi->Inform() ) );
}

QString MediaInfo::GetLibraryVersion() const
{
    return( QString::fromStdWString( mi->Option( __T( "Info_Version" ) ) ) );
}
