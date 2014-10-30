#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>

class ToolBar : public QToolBar
{
    Q_OBJECT

    public:
        ToolBar( QWidget* parent = nullptr );

    signals:
        void actionAdd();
        void actionEdit();
        void actionRemove();
        void actionRandom();
        void actionSearch();
        void actionExit();

    private slots:
        void actionAddTriggered();
        void actionEditTriggered();
        void actionRemoveTriggered();
        void actionRandomTriggered();
        void actionSearchTriggered();
        void actionExitTriggered();
};

#endif // TOOLBAR_H
