
#ifndef FILMSVIEWGRID_H
#define FILMSVIEWGRID_H

#include <QListView>

#include "../abstractfilmsview.h"
#include "filmsviewgridproxymodel.h"

class FilmsViewGrid : public QListView, public AbstractFilmsView
{
    Q_OBJECT

    public:
        explicit FilmsViewGrid( QWidget* parent = nullptr );

        void setModel( QAbstractItemModel* model ) override;

    public slots:
        void LoadSettings() override { ReloadSettings(); }
        void ReloadSettings() override;

        void ScrollToCurrentItem() override { scrollTo( currentIndex() ); }

        int GetRowCount() { return( proxyModel->rowCount() ); }
        QModelIndex GetCurrentIndex() { return( proxyModel->mapToSource( currentIndex() ) ); }
        QModelIndexList GetSelectedItemsList();

        void SetCurrentRow( int r ) { setCurrentIndex( model()->index( r, 0 ) ); }

    signals:
        void CurrentChanged( const QModelIndex& );
        void ContextMenuRequested( const QPoint&, const QModelIndex& );

    protected:
        void updateGeometries() override;

    private slots:
        void MapCurrentChanged( const QModelIndex& index );

    private:
        FilmsViewGridProxyModel* proxyModel;
};

#endif // FILMSVIEWGRID_H

