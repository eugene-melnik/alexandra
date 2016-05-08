
#ifndef FILMDETAILEDINFODELEGATE_H
#define FILMDETAILEDINFODELEGATE_H


#include <QPainter>
#include <QStyledItemDelegate>


#include "filmdetailedinfo.h"
#include "filmslist/filmslistproxymodel.h"
#include "tools/debug.h"


class FilmDetailedInfoDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit FilmDetailedInfoDelegate( QObject* parent = nullptr )
            : QStyledItemDelegate( parent ), filmInfo( new FilmDetailedInfo() ) {}

        virtual ~FilmDetailedInfoDelegate()
        {
            delete filmInfo;
        }

        void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const override
        {
              // Highlight if selected
            if( option.state & QStyle::State_Selected )
            {
                painter->fillRect( option.rect, option.palette.highlight() );
            }

            const FilmsListProxyModel* model = static_cast<const FilmsListProxyModel*>( index.model() );
            int row = index.row();

            filmInfo->SetPoster( model->index( row, FilmItem::PosterColumn ).data( Qt::DecorationRole ).value<QPixmap>() );

            QString title = model->index( row, FilmItem::TitleColumn ).data().toString();
            int year = model->index( row, FilmItem::YearColumn ).data().toInt();
            filmInfo->SetTitle( title, year );

            filmInfo->SetViewed( model->index( row, FilmItem::IsViewedColumn ).data( Qt::DecorationRole ).value<QPixmap>() );
            filmInfo->SetFavourite( model->index( row, FilmItem::IsFavouriteColumn ).data( Qt::DecorationRole ).value<QPixmap>() );
            filmInfo->SetRating( model->index( row, FilmItem::RatingColumn ).data( Qt::DecorationRole ).value<QPixmap>() );

            filmInfo->SetCountry( model->index( row, FilmItem::CountryColumn ).data().toString() );
            filmInfo->SetGenres( model->index( row, FilmItem::GenreColumn ).data().toString() );
            filmInfo->SetDirector( model->index( row, FilmItem::DirectorColumn ).data().toString() );
            filmInfo->SetStarring( model->index( row, FilmItem::StarringColumn ).data().toString() );

              // https://bugreports.qt.io/browse/QTBUG-26694
            QPoint offset = painter->deviceTransform().map( option.rect.topLeft() );
            QRegion region( 0, 0, option.rect.width(), option.rect.height() );

            filmInfo->setGeometry( option.rect );
            filmInfo->render( painter, offset, region, QWidget::RenderFlag::DrawChildren );
        }

        QSize sizeHint( const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/ ) const override
        {
            return( QSize(0, filmInfo->minimumHeight()) );
        }

    private:
        FilmDetailedInfo* filmInfo;
};


#endif // FILMDETAILEDINFODELEGATE_H

