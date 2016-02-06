
#ifndef FILMSVIEWGRIDPROXYMODEL_H
#define FILMSVIEWGRIDPROXYMODEL_H

#include <QCache>
#include <QPixmap>
#include <QIdentityProxyModel>

#include "alexandrasettings.h"

class FilmsViewGridProxyModel : public QIdentityProxyModel
{
    Q_OBJECT

    public:
        explicit FilmsViewGridProxyModel( QObject* parent = nullptr )
          : QIdentityProxyModel( parent ),
            settings( AlexandraSettings::GetInstance() ),
            pixmapCache( new QCache<QString,QPixmap>() ) {}

        ~FilmsViewGridProxyModel() { delete pixmapCache; }

        void SetCacheSize( int size ) { pixmapCache->setMaxCost( size + 1 ); }

        int columnCount( const QModelIndex& ) const override { return( 1 ); }
        QVariant data( const QModelIndex& index, int role ) const override;

    private:
        AlexandraSettings* settings;
        QCache<QString,QPixmap>* pixmapCache;
};

#endif // FILMSVIEWGRIDPROXYMODEL_H
