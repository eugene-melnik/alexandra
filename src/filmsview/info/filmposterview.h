
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

        void setSelectionModel( QItemSelectionModel* selectionModel ) override;

        QRect visualRect( const QModelIndex& ) const override { return( rect() ); }
        void scrollTo( const QModelIndex&, ScrollHint ) override {}
        QModelIndex indexAt( const QPoint& ) const override { return( currentIndex() ); }

    protected:
        int horizontalOffset() const override { return( 0 ); }
        int verticalOffset() const override { return( 0 ); }

        bool isIndexHidden( const QModelIndex& ) const override { return( false ); }

        void setSelection( const QRect&, QItemSelectionModel::SelectionFlags ) override {}
        QRegion visualRegionForSelection( const QItemSelection& ) const override { return( rect() ); }

        QModelIndex	moveCursor( CursorAction, Qt::KeyboardModifiers ) override { return( currentIndex() ); }

    private slots:
        void ShowSelected( const QModelIndex& current, const QModelIndex& /* previous */ );

    private:
        QLabel* image;
};

#endif // FILMPOSTERVIEW_H
