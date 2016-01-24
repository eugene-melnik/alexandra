
#ifndef FILMPOSTERVIEW_H
#define FILMPOSTERVIEW_H

#include <QAbstractItemView>
#include <QLabel>

class FilmPosterView : public QAbstractItemView
{
    Q_OBJECT

    public:
        explicit FilmPosterView( QWidget* parent = nullptr );

        void SetPixmap( const QPixmap& pixmap );
        void Clear();

        void setSelectionModel( QItemSelectionModel* selectionModel );

        QRect visualRect( const QModelIndex& /* index */ ) const { return( rect() ); }
        void scrollTo( const QModelIndex& /* index */, ScrollHint /* hint = EnsureVisible */ ) {}
        QModelIndex indexAt( const QPoint& /* point */ ) const { return( currentIndex() ); }

    protected:
        int horizontalOffset() const { return( 0 ); }
        int verticalOffset() const { return( 0 ); }

        bool isIndexHidden( const QModelIndex& ) const { return( false ); }

        void setSelection( const QRect&, QItemSelectionModel::SelectionFlags ) {}
        QRegion visualRegionForSelection( const QItemSelection& ) const { return( rect() ); }

        QModelIndex	moveCursor( CursorAction, Qt::KeyboardModifiers ) { return( currentIndex() ); }

    private slots:
        void ShowSelected( const QModelIndex& current, const QModelIndex& /* previous */ );

    private:
        QLabel* image;
};

#endif // FILMPOSTERVIEW_H
