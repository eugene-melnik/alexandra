#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "ui_settingswindow.h"

#include <QDialog>

class SettingsWindow : public QDialog, public Ui::SettingsWindow
{
    Q_OBJECT

    public:
        SettingsWindow( QWidget* parent = nullptr );

    private:
        void ConfigureLanguageCB();
        void ConfigureStyleCB();
        void ConfigureToolbarStyleCB();
};

#endif // SETTINGSWINDOW_H
