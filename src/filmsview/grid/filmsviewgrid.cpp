
#include "filmsviewgrid.h"

#include <QScrollBar>


FilmsViewGrid::FilmsViewGrid( QWidget* parent ) : QListView( parent ),
    proxyModel( new FilmsViewGridProxyModel( this ) )
{
    QListView::setModel( proxyModel );

      // Appearance
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setEditTriggers( QAbstractItemView::NoEditTriggers );
    setContextMenuPolicy( Qt::CustomContextMenu );
    setResizeMode( QListView::Adjust );
    setViewMode( QListView::IconMode );
    setWrapping( true );
    setSpacing( 5 );

//    setVerticalScrollMode( QAbstractItemView::ScrollPerPixel ); // WTF: Qt bug?

      // Signals
    connect( this, &QListView::customContextMenuRequested, this, [this] (const QPoint& pos)
    {
        emit ContextMenuRequested( pos, proxyModel->mapToSource( currentIndex() ) );
    });
}


void FilmsViewGrid::setModel( QAbstractItemModel* model )
{
    proxyModel->setSourceModel( model );

    connect( selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(MapCurrentChanged(QModelIndex)) );

    connect( model, &QAbstractItemModel::modelReset, this, [this, model]
    {
        proxyModel->SetCacheSize( model->rowCount() );
    } );

    connect( model, &QAbstractItemModel::dataChanged, this, [this]
    {
        QModelIndex index = currentIndex();
        selectionModel()->currentChanged( index, index );
    } );
}


void FilmsViewGrid::ReloadSettings()
{
    setStyleSheet( QString( "font-size: %1pt" ).arg( AlexandraSettings::GetInstance()->GetGridFontSize() ) );
}


void FilmsViewGrid::updateGeometries()
{
    QListView::updateGeometries();
    verticalScrollBar()->setSingleStep( AlexandraSettings::GetInstance()->GetGridItemSize() / 5 );
}


void FilmsViewGrid::MapCurrentChanged( const QModelIndex& index )
{
    emit CurrentChanged( proxyModel->mapToSource(index) );
}
