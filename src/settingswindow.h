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
        void SelectColorUnavailable();

    private:
        void ConfigureApplicationTab();
        void ConfigureDatabaseTab();

        // Variables
        QSettings* settings;

        // Constants
        typedef struct {
            QString name;
            Qt::ToolButtonStyle style;
        } ToolStyle;

        const QList<ToolStyle> toolStyles = { { tr("Icon only"),            Qt::ToolButtonIconOnly },
                                              { tr("Text only"),            Qt::ToolButtonTextOnly },
                                              { tr("Text beside icon"),     Qt::ToolButtonTextBesideIcon },
                                              { tr("Text under icon"),      Qt::ToolButtonTextUnderIcon },
                                              { tr("<Follow system style>"),Qt::ToolButtonFollowStyle } };
};

#endif // SETTINGSWINDOW_H
