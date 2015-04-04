
#include "filmsviewgrid.h"
#include "filmsviewgriditem.h"

#include <QHeaderView>
#include <QPalette>

FilmsViewGrid::FilmsViewGrid( QWidget* parent ) : QTableWidget( parent )
{
    // Appearance
    setEditTriggers( QAbstractItemView::NoEditTriggers );
    setGridStyle( Qt::NoPen );
    setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );
    setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
    setSelectionBehavior( QAbstractItemView::SelectItems );

    horizontalHeader()->setVisible( false );
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    verticalHeader()->setVisible( false );

    // Signals
    connect( this, SIGNAL( cellClicked(int,int) ), this, SLOT( ItemClickedSlot(int,int) ) );
    connect( this, SIGNAL( cellActivated(int,int) ), this, SLOT( ItemClickedSlot(int,int) ) );
    connect( this, SIGNAL( cellDoubleClicked(int,int) ), this, SLOT( ItemDoubleClickedSlot(int,int) ) );
}

void FilmsViewGrid::LoadSettings( AlexandraSettings* s )
{
    settings = s;
    setColumnCount( s->GetGridViewColumnCount() );
}

void FilmsViewGrid::ReloadSettings( AlexandraSettings* s )
{
    setColumnCount( s->GetGridViewColumnCount() );
}

void FilmsViewGrid::AddItem( const Film& film )
{
    setRowCount( (itemsCount + 1) / columnCount() + 1 );
    SetItem( itemsCount++, &film );
}

void FilmsViewGrid::AddItem( const Film& film, QColor background )
{
    setRowCount( (itemsCount + 1) / columnCount() + 1 );
    SetItem( itemsCount++, &film,background );
}

void FilmsViewGrid::SetItem( int n, const Film* film, QColor background )
{
    FilmViewGridItem* item = new FilmViewGridItem( film, settings, this );

    if( background.isValid() )
    {
        QPalette pal( palette() );
        pal.setColor( QPalette::Background, background );
        item->setBackgroundRole(QPalette::Background);
        item->setAutoFillBackground( true );
        item->setPalette( pal );
    }

    int row = n / columnCount();
    int column = n % columnCount();
    setCellWidget( row, column, item );
}

void FilmsViewGrid::Clear()
{
    clearContents();
    itemsCount = 0;
    setRowCount( itemsCount );
}

void FilmsViewGrid::SelectItem( Film film )
{
    SelectItem( film.GetTitle() );
}

void FilmsViewGrid::SelectItem( QString title )
{
    for( int i = 0; i < itemsCount; i++ )
    {
        QWidget* widget = cellWidget( i / columnCount(), i % columnCount() );
        FilmViewGridItem* item = dynamic_cast<FilmViewGridItem*>( widget );

        if( item->GetTitle() == title )
        {
            SetCurrentItemIndex( i );
            return;
        }
    }

    SetCurrentItemIndex( 0 );
}

void FilmsViewGrid::SelectRandomItem()
{
    if( itemsCount > 0 )
    {
        int n;

        do
        {
            n = qrand() % rowCount();
        }
        while( n == GetCurrentItemIndex() );

        SetCurrentItemIndex( n );
    }
}

int FilmsViewGrid::GetItemsCount() const
{
    return( itemsCount );
}

int FilmsViewGrid::GetCurrentItemIndex() const
{
    int row = currentIndex().row();
    int column = currentIndex().column();
    return( row * columnCount() + column );
}

void FilmsViewGrid::SetCurrentItemIndex( int i )
{
    if( itemsCount > 0 )
    {
        if( (i > itemsCount) || (i < 0) )
        {
            i = 0;
        }

        setCurrentCell( i / columnCount(), i % columnCount() );
        cellClicked( i / columnCount(), i % columnCount() );
        resizeColumnsToContents();
        resizeRowsToContents();
    }
}

void FilmsViewGrid::ItemClickedSlot( int row, int column )
{
    int itemIndex = row * columnCount() + column;

    if( itemIndex >= itemsCount )
    {
        itemIndex = itemsCount - 1;
        SetCurrentItemIndex( itemIndex );
    }

    QWidget* widget = cellWidget( itemIndex / columnCount(), itemIndex % columnCount() );
    FilmViewGridItem* item = dynamic_cast<FilmViewGridItem*>( widget );

    emit ItemClicked( item->GetTitle() );
}

void FilmsViewGrid::ItemDoubleClickedSlot( int row, int column )
{
    int itemIndex = row * columnCount() + column;

    if( itemIndex < itemsCount )
    {
        QWidget* widget = cellWidget( itemIndex / columnCount(), itemIndex % columnCount() );
        FilmViewGridItem* item = dynamic_cast<FilmViewGridItem*>( widget );

        emit ItemDoubleClicked( item->GetTitle() );
    }
}
