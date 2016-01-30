
#include "filmposterview.h"
#include "filmslist/filmitem.h"
#include "tools/debug.h"

#include <QAbstractProxyModel>
#include <QResizeEvent>


void FilmPosterView::ShowInformation( const QModelIndex& index )
{
    DebugPrintFuncA( "FilmPosterView::ShowInformation", index.row() );

    const QAbstractProxyModel* model = static_cast<const QAbstractProxyModel*>( index.model() );
    QVariant data = model->index( index.row(), FilmItem::PosterColumn ).data( Qt::DecorationRole );

    if( data.isValid() )
    {
        SetPixmap( data.value<QPixmap>() );
    }
    else
    {
        Clear();
    }

    DebugPrintFuncDone( "FilmPosterView::ShowInformation" );
}


void FilmPosterView::Clear()
{
    SetPixmap( QPixmap( ":/standart-poster" ) );
}


void FilmPosterView::resizeEvent( QResizeEvent* event )
{
    QLabel::resizeEvent( event );

    if( event->oldSize().width() != event->size().width() &&
        pixmap() != nullptr )
    {
        SetPixmap( *pixmap() );
    }
}


void FilmPosterView::showEvent( QShowEvent* event )
{
    QLabel::showEvent( event );
    Clear();
}


void FilmPosterView::SetPixmap( const QPixmap& pixmap )
{
    QPixmap p = pixmap.scaledToWidth( width(), Qt::SmoothTransformation );
    setMinimumHeight( p.height() );
    setPixmap( p );
}

