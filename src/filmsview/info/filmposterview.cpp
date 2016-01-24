
#include "filmposterview.h"
#include "filmslist/filmslistmodel.h"
#include "tools/debug.h"

#include <QBoxLayout>


FilmPosterView::FilmPosterView( QWidget* parent ) : QAbstractItemView( parent )
{
    image = new QLabel( this );

    QBoxLayout* layout = new QBoxLayout( QBoxLayout::LeftToRight, this );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( image );

    setLayout( layout );
    Clear();
}


void FilmPosterView::SetPixmap( const QPixmap& pixmap )
{
    QPixmap p = pixmap.scaledToWidth( width(), Qt::SmoothTransformation );
    setMinimumHeight( p.height() );
    image->setPixmap( p );
}


void FilmPosterView::Clear()
{
    SetPixmap( QPixmap( ":/standart-poster" ) );
}


void FilmPosterView::setSelectionModel( QItemSelectionModel* selectionModel )
{
    QAbstractItemView::setSelectionModel( selectionModel );
    connect( selectionModel, &QItemSelectionModel::currentRowChanged, this, &FilmPosterView::ShowSelected );
}

void FilmPosterView::ShowSelected( const QModelIndex& current, const QModelIndex& /* previous */ )
{
    if( current.isValid() )
    {
        DebugPrintFuncA( "FilmPosterView::ShowSelected", current.row() );
        QVariant data = model()->index( current.row(), FilmsListModel::PosterColumn ).data( Qt::DecorationRole );

        if( data.isValid() )
        {
            SetPixmap( data.value<QPixmap>() );
        }
        else
        {
            Clear();
        }

        DebugPrintFuncDone( "FilmPosterView::ShowSelected" );
    }
}
