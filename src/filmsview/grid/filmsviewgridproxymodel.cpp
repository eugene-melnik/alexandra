
#include "filmsviewgridproxymodel.h"
#include "filmslist/filmslistmodel.h"


QVariant FilmsViewGridProxyModel::data( const QModelIndex& index, int role ) const
{
    if( index.isValid() )
    {
        QAbstractItemModel* model = sourceModel();
        int row = index.row();

        switch( role )
        {
            case Qt::TextAlignmentRole :
            {
                return( Qt::AlignCenter );
            }

            case Qt::DecorationRole :
            {
                QString posterFileName = model->index( row, FilmItem::PosterColumn ).data().toString();

                if( pixmapCache->contains(posterFileName) )
                {
                    return( *pixmapCache->object(posterFileName) );
                }

                QVariant data = model->index( row, FilmItem::PosterColumn ).data( Qt::DecorationRole );

                if( data.isValid() )
                {
                    QPixmap* pixmap = new QPixmap( data.value<QPixmap>().scaledToHeight( settings->GetGridItemSize(),
                                                                                         Qt::SmoothTransformation ) );
                    pixmapCache->insert( posterFileName, pixmap );
                    return( *pixmap );
                }
            }

            case Qt::ToolTipRole :
            {
                if( settings->GetGridShowTooltip() == true )
                {
                    QString tooltip, title, text;

                    for( int column = 1; column < model->columnCount() - 2; ++column ) // without filenames
                    {
                        text = model->index( row, column ).data().toString();

                        if( text.length() > 1 )
                        {
                            title = model->headerData( column, Qt::Horizontal ).toString();
                            tooltip += QString( "<b>%1:</b> %2<br/>" ).arg( title, text );
                        }
                    }

                    return( tooltip );
                }
            }

            case Qt::SizeHintRole :
            {
                int width = settings->GetGridItemSize() + settings->GetGridItemSize() * 0.1; // +10%
                int height = settings->GetGridItemSize() + settings->GetGridFontSize() * 2;  // +2* font size
                return( QSize( width, height ) );
            }
        }
    }

    return QIdentityProxyModel::data( index, role );
}

