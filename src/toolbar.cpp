#include "toolbar.h"

#include <QAction>
#include <QIcon>

ToolBar::ToolBar( QWidget* parent ) : QToolBar( parent )
{
    // Add film
    QAction* tbaAdd = new QAction( QIcon( ":/tool/add" ), tr( "Add" ), this );
    addAction( tbaAdd );
    connect( tbaAdd, SIGNAL( triggered() ), this, SLOT( actionAddTriggered() ) );

    // Edit film
    QAction* tbaEdit = new QAction( QIcon( ":/tool/edit" ), tr( "Edit" ), this );
    addAction( tbaEdit );
    connect( tbaEdit, SIGNAL( triggered() ), this, SLOT( actionEditTriggered() ) );

    // Remove film
    QAction* tbaRemove = new QAction( QIcon( ":/tool/delete" ), tr( "Remove" ), this );
    addAction( tbaRemove );
    connect( tbaRemove, SIGNAL( triggered() ), this, SLOT( actionRemoveTriggered() ) );

    addSeparator();

    // Random
    QAction* tbaRandom = new QAction( QIcon( ":/tool/random" ), tr( "Random" ), this );
    addAction( tbaRandom );
    connect( tbaRandom, SIGNAL( triggered() ), this, SLOT( actionRandomTriggered() ) );

    // Search
    QAction* tbaSearch = new QAction( QIcon( ":/tool/find" ), tr( "Search" ), this );
    addAction( tbaSearch );
    connect( tbaSearch, SIGNAL( triggered() ), this, SLOT( actionSearchTriggered() ) );

    addSeparator();

    // Exit
    QAction* tbaExit = new QAction( QIcon( ":/action/exit" ), tr( "Exit" ), this );
    addAction( tbaExit );
    connect( tbaExit, SIGNAL( triggered() ), this, SLOT( actionExitTriggered() ) );
}

void ToolBar::actionAddTriggered()
{
    emit actionAdd();
}

void ToolBar::actionEditTriggered()
{
    emit actionEdit();
}

void ToolBar::actionRemoveTriggered()
{
    emit actionRemove();
}

void ToolBar::actionRandomTriggered()
{
    emit actionRandom();
}

void ToolBar::actionSearchTriggered()
{
    emit actionSearch();
}

void ToolBar::actionExitTriggered()
{
    emit actionExit();
}
