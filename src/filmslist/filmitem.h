
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

        void AppendChild( FilmItem* item ) { childItems.append( item ); }
        void SetHighlightColor( const QColor& color ) { highlightColor = color; }
        void RemoveChildren();

        int    GetColumnCount() const { return( columnsData.size() ); }
        int    GetChildCount() const { return( childItems.size() ); }
        int    GetRow() const;
        QColor GetHighlightColor() const { return( highlightColor ); }

        FilmItem* GetParent() { return( parentItem ); }
        FilmItem* GetChild( int row ) { return( childItems.value( row  ) ); }
        QVariant  GetColumnData( int column ) const { return( columnsData.value( column ) ); }

///        QList<FilmItem*>& GetChildren() { return( childItems ); }

          // Static
        static QString GetRandomHash();
        static QString ClearTitle( const QString& title );

    private:
        FilmItem*        parentItem;
        QList<FilmItem*> childItems;

        FilmType        filmType;
        QList<QVariant> columnsData;
        QColor          highlightColor;
        bool            isPosterExists;
};


  // Must be quick

inline void FilmItem::RemoveChildren()
{
    qDeleteAll( childItems );
    childItems.clear();
}


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
