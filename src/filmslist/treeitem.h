#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

class TreeItem
{
    public:
        TreeItem( const QList<QVariant>& data, TreeItem* parent = nullptr )
            : itemData( data ), parentItem( parent ) {}

        ~TreeItem()
        {
            qDeleteAll( childItems );
        }

        void AppendChild( TreeItem* item )
        {
            childItems.append( item );
        }
        void RemoveChildren()
        {
            qDeleteAll( childItems );
            childItems.clear();
        }

        int GetColumnCount() const
        {
            return( itemData.size() );
        }
        int GetChildCount() const
        {
            return( childItems.size() );
        }
        int GetRow() const
        {
            if( parentItem != nullptr )
            {
                return( parentItem->childItems.indexOf( const_cast<TreeItem*>( this ) ) );
            }
            else
            {
                return( 0 );
            }
        }

        TreeItem* GetParent()
        {
            return( parentItem );
        }
        TreeItem* GetChild( int row )
        {
            return( childItems.value( row  ) );
        }
        QList<TreeItem*>& GetChildren()
        {
            return( childItems );
        }
        QVariant  GetData( int column ) const
        {
            return( itemData.value( column ) );
        }

    private:
        QList<TreeItem*>  childItems;
        QList<QVariant>   itemData;
        TreeItem*         parentItem;
};

#endif // TREEITEM_H
