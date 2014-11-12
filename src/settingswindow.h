#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "ui_settingswindow.h"

#include <QDialog>
#include <QShowEvent>
#include <QSettings>

class SettingsWindow : public QDialog, public Ui::SettingsWindow
{
    Q_OBJECT

    public:
        SettingsWindow( QSettings* s, QWidget* parent = nullptr );

    protected:
        void showEvent( QShowEvent* event );

    signals:
        void SettingsChanged();

    private slots:
        void OkButtonClicked();
        void SetDefaultExternalPlayer();
        void OpenDatabaseFile();
        void SelectColorViewed();
        void SelectColorFavourite();
        void SelectColorUnavailable();

    private:
        void ConfigureApplicationTab();
        void ConfigureLanguageCB();
        void ConfigureStyleCB();
        void ConfigureToolbarStyleCB();

        void ConfigureDatabaseTab();

        void ConfigureFilmslistTab();

        QSettings* settings;
};

#endif // SETTINGSWINDOW_H
