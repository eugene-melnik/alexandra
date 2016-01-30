
#ifndef FILMITEM_H
#define FILMITEM_H

#include <QColor>
#include <QCryptographicHash>
#include <QList>
#include <QVariant>

class FilmItem
{
    public:
        FilmItem( const QList<QVariant>& data, FilmItem* parent = nullptr )
            : parentItem( parent ), columnsData( data ) {}

        ~FilmItem() { qDeleteAll( childItems ); }

        /* TODO: describe */

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

        void AppendChild( FilmItem* item ) { childItems.append( item ); }
        void RemoveChildren();

        void SetIsFileExists( Existing exists ) { isFileExists = exists; }
        void SetIsPosterExists( Existing exists ) { isPosterExists = exists; }

        int GetColumnCount() const { return( columnsData.size() ); }
        int GetChildCount() const { return( childItems.size() ); }
        int GetRow() const;

        FilmItem* GetParent() { return( parentItem ); }
        FilmItem* GetChild( int row ) { return( childItems.value( row  ) ); }
        QVariant  GetColumnData( int column ) const { return( columnsData.value( column ) ); }

        bool GetIsFilmViewed() const;
        bool GetIsFilmFavourite() const;
        Existing GetIsFileExists() const { return( isFileExists ); }
        Existing GetIsPosterExists() const { return( isPosterExists ); }

          // Static
        static QString GetRandomHash();
        static QString ClearTitle( const QString& title );

    private:
        FilmItem*        parentItem;
        QList<FilmItem*> childItems;

        FilmType filmType; ///
        Existing isFileExists = Unknown;
        Existing isPosterExists = NotExists; ///
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
