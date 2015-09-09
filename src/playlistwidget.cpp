
#include "playlistwidget.h"

#include <QAction>
#include <QListWidgetItem>
#include <QMenu>

PlayListWidget::PlayListWidget( QWidget* parent ) : QListWidget( parent )
{
    setAlternatingRowColors( true );
    setContextMenuPolicy( Qt::CustomContextMenu );
    setDragDropMode( QAbstractItemView::InternalMove );

    connect( this, &PlayListWidget::customContextMenuRequested, this, &PlayListWidget::ShowContextMenu );
}

void PlayListWidget::AddItem( const QString& title , const QString& filePath )
{
    QListWidgetItem* item = new QListWidgetItem( title, this );
    item->setToolTip( filePath );
    addItem( item );
    setCurrentItem( item );
}

void PlayListWidget::Clear()
{
    clear();
    emit Cleared();
}

QStringList PlayListWidget::GetPathes() const
{
    QStringList pathes;

    for( int i = 0; i < count(); i++ )
    {
        pathes.append( item( i )->toolTip() );
    }

    return( pathes );
}

void PlayListWidget::ShowContextMenu( const QPoint& pos )
{
    if( count() > 0 && currentRow() >= 0 )
    {
        QMenu menu;
        menu.addAction( QIcon( ":/tool/delete" ), tr( "Remove from playlist" ), this, SLOT( RemoveFromList() ) );
        menu.addAction( tr( "Clear all" ), this, SLOT( Clear() ) );
        menu.exec( mapToGlobal( pos ) );
    }
}

void PlayListWidget::RemoveFromList()
{
    model()->removeRow( currentRow() );

    if( count() == 0 )
    {
        emit Cleared();
    }
}
