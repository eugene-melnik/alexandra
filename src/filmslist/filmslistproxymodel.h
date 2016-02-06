
#ifndef FILMSLISTPROXYMODEL_H
#define FILMSLISTPROXYMODEL_H

#include <QList>
#include <QSortFilterProxyModel>

#include "filmslist/filmitem.h"

class FilmsListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    public:
        explicit FilmsListProxyModel( QObject* parent = nullptr ) : QSortFilterProxyModel( parent ) {}

        QVariant data( const QModelIndex &index, int role ) const override;

    public slots:
        const FilmItem* GetFilmItemByIndex( const QModelIndex& index ) const
        {
            return( static_cast<FilmItem*>( mapToSource(index).internalPointer() ) );
        }

        QString GetFilmTitleByIndex( const QModelIndex& index ) const
        {
            const FilmItem* film = GetFilmItemByIndex(index);

            if( film != nullptr )
            {
                return( film->GetColumnData( FilmItem::TitleColumn ).toString() );
            }
            else
            {
                return( QString() );
            }
        }

        void SetFilter( const QString& string, const QList<int>& columns )
        {
            filterString = string;
            filterColumns = columns;
            invalidateFilter();
        }

    protected:
        bool filterAcceptsRow( int sourceRow, const QModelIndex& sourceParent ) const
        {
            if( filterString.isEmpty() ) return( true );
            if( filterColumns.isEmpty() ) return( false );

            for( int column : filterColumns )
            {
                QString columnString = sourceModel()->index( sourceRow, column, sourceParent ).data().toString();
                if( columnString.contains( filterString, Qt::CaseInsensitive ) ) return( true );
            }

            return( false );
        }

    private:
        QList<int> filterColumns;
        QString filterString;
};

#endif // FILMSLISTPROXYMODEL_H
