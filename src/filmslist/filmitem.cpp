
#include "filmitem.h"


void FilmItem::RemoveChildren()
{
    qDeleteAll( childItems );
    childItems.clear();
}


bool FilmItem::GetIsFilmViewed() const
{
    bool viewed = GetColumnData( IsViewedColumn ).toString() == "+" ? true : false;
    return( viewed );
}


bool FilmItem::GetIsFilmFavourite() const
{
    bool viewed = GetColumnData( IsFavouriteColumn ).toString() == "+" ? true : false;
    return( viewed );
}


QString FilmItem::GetRandomHash()
{
    QByteArray hash = QCryptographicHash::hash( QByteArray::number( qrand() ), QCryptographicHash::Sha1 );
    return( QString( hash.toHex() ) );
}


QString FilmItem::ClearTitle( const QString& title )
{
    QString newTitle = title;

      // List of words (of course not all), which may be present
      // in the file name, but are not part of the title
    QStringList wordsToRemove =
    {
        "DHT-Movies",  "TheaterRip",  "BlueBird",  "HDTV720",  "HDTVRip",  "NeroAVC",  "UNRATED",  "BluRay",
        "DVDRip",      "HDclub",      "HDRip",     "WEB-DL",   "1080p",    "BDRip",    "720p",     "H264",
        "HDTV",        "HiDt",        "x264",      "AC3",      "AVC",      "AVO",      "DTS",      "Eng",
        "JRG",         "MKV",         "MVO",       "Fra",      "Rus",      "Ukr",      "HD"
    };

    for( const QString& word : wordsToRemove )
    {
        newTitle.replace( word, "", Qt::CaseInsensitive );
    }

    QStringList regexpsToRemove =
    {
        "(185[0-9]|18[6-9][0-9]|19[0-9]{2}|200[0-9]|201[0-9])", // Years between 1850 and 2019
        "\\[(.+)\\]|\\((.+)\\)",                                // Round and square brackets with the contents
        "\\(\\)|\\[\\]",                                        // Round and square brackets whithout contents
        "\\.|_|\\-",                                            // Dots, underscores and dashes (hyphens)
        "[0-9][xX]",                                            // Phrases like "NxSomething", where N is a number
        " {1,}",                                                // Multiple spaces
    };

    for( const QString& regexp : regexpsToRemove )
    {
        newTitle.replace( QRegExp(regexp), " " );
    }

      // Spaces at begin and end of the string
    newTitle = newTitle.trimmed();

    if( newTitle.length() > 1 )
    {
        return( newTitle );
    }
    else
    {
        return( title );
    }
}

