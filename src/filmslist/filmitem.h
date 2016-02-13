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
#include <QList>
#include <QVariant>


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

            ColumnCount
        };

        enum Existing
        {
            Unknown,
            Exists,
            NotExists
        };

          // Set

        void AppendChild( FilmItem* item );

        void RemoveChild( FilmItem* item ) { delete childItems.takeAt( childItems.indexOf(item) ); }
        void RemoveChildren();

        void SetParent( FilmItem* parent ) { parentItem = parent; }
        void SetColumnData( int column, const QVariant& data ) { columnsData[column] = data; }

        void SetFilmId( QString id ) { filmId = id; }
        void SetFilmType( FilmType type ) { filmType = type; }
        void SetIsFileExists( Existing exists ) { isFileExists = exists; }
        void SetIsPosterExists( Existing exists ) { isPosterExists = exists; }

          // Get

        int GetColumnCount() const { return( columnsData.size() ); }
        int GetChildrenCount() const { return( childItems.size() ); }
        int GetRow() const;

        FilmItem* GetParent() { return( parentItem ); }
        FilmItem* GetChild( int row ) { return( childItems.value(row) ); }
        QVariant  GetColumnData( int column ) const { return( columnsData.value(column) ); }

        bool     GetIsFilmViewed() const { return( GetColumnData(IsViewedColumn).toBool() ); }
        bool     GetIsFilmFavourite() const { return( GetColumnData(IsFavouriteColumn).toBool() ); }
        QString  GetFilmId() const { return( filmId ); }
        FilmType GetFilmType() const { return( filmType ); }
        Existing GetIsFileExists() const { return( isFileExists ); }
        Existing GetIsPosterExists() const { return( isPosterExists ); }

        QString GetTitle() const { return( columnsData.at(TitleColumn).toString() ); }
        QString GetFileName() const { return( columnsData.at(FileNameColumn).toString() ); }

          // Static

        static QString GetRandomHash();
        static QString GetClearedTitle( const QString& title, int* year = nullptr );

    private:
        FilmItem* parentItem = nullptr;
        QList<FilmItem*> childItems;

        QString  filmId;
        FilmType filmType = Movie;
        Existing isFileExists = Unknown;
        Existing isPosterExists = NotExists;
        QList<QVariant> columnsData;
};


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


#endif // FILMITEM_H
