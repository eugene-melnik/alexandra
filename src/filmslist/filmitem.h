/*************************************************************************************************
 *                                                                                                *
 *  file: filmitem.h                                                                              *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2016 Eugene Melnik <jeka7js@gmail.com>                                     *
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

#ifndef FILMITEM_H
#define FILMITEM_H


#include <QColor>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QList>
#include <QVariant>


#include "alexandrasettings.h"


class FilmItem
{
    public:
        FilmItem();

        FilmItem( const QList<QVariant>& data, FilmItem* parent = nullptr )
            : parentItem( parent ), columnsData( data ) {}

        FilmItem( const FilmItem& item ) { *this = item; }

        ~FilmItem() { qDeleteAll( childItems ); }

        FilmItem& operator = ( const FilmItem& item );

        /* TODO: describe */

          // Types

        enum FilmType
        {
            Movie,
            Serial,
            SerialSeason,
            SerialEpisode
        };

        enum Column
        {
            TitleColumn,
            OriginalTitleColumn,
            TaglineColumn,
            YearColumn,
            GenreColumn,
            CountryColumn,
            DirectorColumn,
            ProducerColumn,
            ScreenwriterColumn,
            ComposerColumn,
            BudgetColumn,
            RatingColumn,
            IsViewedColumn,
            IsFavouriteColumn,
            ViewsCountColumn,
            StarringColumn,
            DescriptionColumn,
            TagsColumn,
            FileNameColumn,
            PosterColumn,

            VisibleColumnCount,

            ViewingDates = VisibleColumnCount,

            AllColumnCount
        };

        enum Existing
        {
            Unknown,
            Exists,
            NotExists
        };

          // Set //

        void AppendChild( FilmItem* item );

        void RemoveChild( FilmItem* item ) { delete childItems.takeAt( childItems.indexOf(item) ); }
        void RemoveChildren();

        void SetParent( FilmItem* parent ) { parentItem = parent; }
        void SetColumnData( int column, const QVariant& data ) { columnsData[column] = data; }

        void SetFilmType( FilmType type ) { filmType = type; }
        void SetIsFileExists( Existing exists ) { isFileExists = exists; }
        void SetIsPosterExists( Existing exists ) { isPosterExists = exists; }

        void IncViewsCounter();
        void ResetViewsCounter();

          // Get //

        int GetColumnCount() const { return( VisibleColumnCount ); }
        int GetChildrenCount() const { return( childItems.size() ); }
        int GetRow() const;

        FilmItem* GetParent() const { return( parentItem ); }
        FilmItem* GetChild( int row ) const { return( childItems.value(row) ); }
        QVariant  GetColumnData( int column ) const { return( columnsData.value(column) ); }

        bool GetIsFilmViewed() const { return( GetColumnData(IsViewedColumn).toBool() ); }
        bool GetIsFilmFavourite() const { return( GetColumnData(IsFavouriteColumn).toBool() ); }

        FilmType GetFilmType() const { return( filmType ); }
        bool GetIsFileExists() const;
        bool GetIsPosterExists() const;

        QString GetTitle() const { return( columnsData.at(TitleColumn).toString() ); }
        QString GetFileName() const { return( columnsData.at(FileNameColumn).toString() ); }
        QString GetPosterFilePath() const;

          // Static //

        static QString GetRandomHash();
        static QString GetClearedTitle( QString title, int* year = nullptr );

    private:
        FilmItem* parentItem = nullptr;
        QList<FilmItem*> childItems;

        FilmType filmType = Movie;
        QList<QVariant> columnsData;

        mutable Existing isFileExists = Unknown;
        mutable Existing isPosterExists = Unknown;

          // Constants
        const QString DatetimeFormat = "yyyy.MM.dd--hh:mm,";
};


typedef QList<FilmItem*> FilmItemList;


  // Must be quick

inline int FilmItem::GetRow() const
{
    if( parentItem != nullptr )
    {
        return( parentItem->childItems.indexOf( const_cast<FilmItem*>(this) ) );
    }
    else
    {
        return( 0 );
    }
}


inline bool FilmItem::GetIsFileExists() const
{
    if( isFileExists == FilmItem::Unknown )
    {
        isFileExists = QFileInfo::exists(GetFileName()) ? FilmItem::Exists : FilmItem::NotExists;
    }

    return( isFileExists == FilmItem::Exists );
}


inline bool FilmItem::GetIsPosterExists() const
{
    if( isPosterExists == FilmItem::Unknown )
    {
        isPosterExists = QFileInfo::exists(GetPosterFilePath()) ? FilmItem::Exists : FilmItem::NotExists;
    }

    return( isPosterExists == FilmItem::Exists );
}


#endif // FILMITEM_H

