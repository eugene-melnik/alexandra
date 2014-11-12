#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include "ui_searchwindow.h"

#include <QDialog>

class SearchWindow : public QDialog, public Ui::SearchWindow
{
    Q_OBJECT

    public:
        SearchWindow( QWidget* parent = nullptr );
};

#endif // SEARCHWINDOW_H
