#include "searchwindow.h"

#include <QStringList>

SearchWindow::SearchWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );
    ConfigureTable();
}

void SearchWindow::ConfigureTable()
{
    QStringList colNames;
    colNames << tr( "Title" )
             << tr( "Year" )
             << tr( "Genre" )
             << tr( "Director" );
    twResult->setColumnCount( colNames.size() );
    twResult->setHorizontalHeaderLabels( colNames );
}
