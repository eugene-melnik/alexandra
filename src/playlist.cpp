
#include "playlist.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

QString PlayList::CreateTempListM3U8() const
{   
    if( pathes.isEmpty() )
    {
        return( QString() );
    }

    QString fileName( QDir::tempPath() + QString( "/tmpList%1.m3u8" ).arg( rand() ) );

    QFile fileToSave( fileName );
    fileToSave.open( QIODevice::WriteOnly );
    QTextStream streamToSave( &fileToSave );

    streamToSave << "#EXTM3U\n";  // m3u header

    for( const QString& p : pathes )
    {
        streamToSave << p.toUtf8() << "\n";
    }

    fileToSave.close();
    return( fileName );
}
