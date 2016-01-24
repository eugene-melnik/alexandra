
#ifndef FILMSVIEWGRID_H
#define FILMSVIEWGRID_H

#include <QListView>

#include "../abstractfilmsview.h"
#include "filmsviewgridproxymodel.h"

class FilmsViewGrid : public QListView, public AbstractFilmsView
{
    public:
        explicit FilmsViewGrid( QWidget* parent = nullptr );

        void setModel( QAbstractItemModel* model ) override;

    public slots:
        void LoadSettings() override;
        void ReloadSettings() override;

    protected:
        void updateGeometries() override;

//    private slots:
//        void ShowHeaderContextMenu( const QPoint& pos );

    private:
        FilmsViewGridProxyModel* proxyModel;
};

#endif // FILMSVIEWGRID_H

