
#ifndef FILMSVIEWGRID_H
#define FILMSVIEWGRID_H

#include <QColor>
#include <QTableWidget>

#include "abstractfilmsview.h"
#include "alexandrasettings.h"
#include "film.h"

class FilmsViewGrid : public QTableWidget, public AbstractFilmsView
{
    Q_OBJECT

    public:
        FilmsViewGrid( QWidget* parent = nullptr );

    public slots:
        void LoadSettings( AlexandraSettings* s );
        void ReloadSettings( AlexandraSettings* s );
        void SaveSettings( AlexandraSettings* s ) const { Q_UNUSED(s) }

        void AddItem( const Film& film );
        void AddItem( const Film& film, QColor background );
        void SetItem( int n, const Film* film, QColor background = QColor() );
        void Clear();

        void SelectItem( Film film );
        void SelectItem( QString title );
        void SelectRandomItem();

        int GetItemsCount() const;
        int GetCurrentItemIndex() const;

        void SetCurrentItemIndex( int i );

    private slots:
        void ItemClickedSlot( int row, int column );
        void ItemDoubleClickedSlot( int row, int column );
        void ContextMenuRequestedSlot( QPoint p );

    signals:
        void ItemClicked( QString );
        void ItemDoubleClicked( QString );
        void ContextMenuRequested( QPoint );

    private:
        AlexandraSettings* settings = nullptr;
        int itemsCount = 0;
};

#endif // FILMSVIEWGRID_H
