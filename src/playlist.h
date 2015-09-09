
#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QStringList>

class PlayList
{
    public:
        PlayList() = delete;
        PlayList( const QStringList& p ) : pathes( p ) {}

        void AddPath( const QString& s ) { pathes.append( s ); }
        void AddPathes( const QStringList& s ) { pathes.append( s ); }
        void Clear() { pathes.clear(); }

        QString CreateTempListM3U8() const;

    private:
        QStringList pathes;
};

#endif // PLAYLIST_H
