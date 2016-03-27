/*************************************************************************************************
 *                                                                                                *
 *  file: settingswindow.h                                                                        *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2016 Eugene Melnik <jeka7js@gmail.com>                                     *
 *                                                                                                *
 *  Alexandra is free software; you can redistribute it and/or modify it under the terms of the   *
 *  GNU General Public License as published by the Free Software Foundation; either version 2 of  *
 *  the License, or (at your option) any later version.                                           *
 *                                                                                                *
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     *
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     *
 *  See the GNU General Public License for more details.                                          *
 *                                                                                                *
 *  You should have received a copy of the GNU General Public License along with this program.    *
 *  If not, see <http://www.gnu.org/licenses/>.                                                   *
 *                                                                                                *
  *************************************************************************************************/

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H


#include "tools/debug.h"
#include "alexandrasettings.h"
#include "ui_settingswindow.h"


#include <functional>
#include <QDialog>
#include <QList>
#include <QMap>
#include <QStringList>


class SettingsWindow : public QDialog, protected Ui::SettingsWindow
{
    Q_OBJECT

    public:
        explicit SettingsWindow( QWidget* parent = nullptr );

    signals:
        void DbSettingsChanged();
        void SettingsChanged();
        void ViewChanged();
        void EraseDatabase();

    private slots:
        void SetIsSettingsChanged() { isSettingsChanged = true; }
        void SetIsViewChanged() { SetIsSettingsChanged(); isViewChanged = true; }
        void SetIsNeedReboot() { SetIsSettingsChanged(); isNeedReboot = true; }
        void SetIsDbSettingsChanged() { SetIsSettingsChanged(); isDbSettingsChanged = true; }

          // Appearance
        void ConfigureAppearanceTab();
        void StyleChanged();
        void SelectFont();
        void SelectFontDefault();
          // Application
        void ConfigureApplicationTab();
        void SelectExternalPlayer();
        void SetDefaultExternalPlayer();
        void OpenDatabaseFile();
        void CreateDatabase();
        void EraseDatabaseQuestion();
        void SelectColorUnavailable();
        void OpenPostersFolder();
          // Shortcuts
        void ConfigureShortcutsTab();
        void ShortcutsKeySequenceHandler();
        void ShortcutsDefaultButtonHandler();
        void ShortcutsClearButtonHandler();
        bool HasDuplicate( QString currentKey, QString objName );
          // Sources
        void ConfigureSourcesTab();

          // Saving
        void SaveSettings();

    private:
          // Variables
        AlexandraSettings* settings;

        bool isSettingsChanged = false;
        bool isDbSettingsChanged = false;
        bool isViewChanged = false;
        bool isNeedReboot = false;

          // Styles
        QStringList appStyles;
        QMap<int,QString> toolStyles;

          // Player double click
        QMap<QString, QString> playerBehaviors;

          // Saving formats
        struct PosterFormat
        {
            QString title;
            QString format;
            int quality;
        };

        const QList<PosterFormat> savingFormats =
        {
            { tr("PNG (compressed)"),   "png",  0 },
            { tr("JPG (quality 95)"),   "jpg", 95 },
            { tr("JPG (quality 85)"),   "jpg", 85 },
            { tr("JPG (quality 75)"),   "jpg", 75 },
            { tr("BMP (uncompressed)"), "bmp", -1 }
        };

          // Shortcuts
        struct Shortcut
        {
            QString defaultKey;
            QKeySequenceEdit* keyEdit;
            QPushButton* buttonDefault;
            QPushButton* buttonClear;
            std::function<QString(AlexandraSettings*)> GetSetting;       // Using: s = var.GetSetting( settings );
            std::function<void(AlexandraSettings*, QString)> SetSetting; // Using: var.SetSetting( settings, value );
        };

        QList<Shortcut> shortcuts;
};


#endif // SETTINGSWINDOW_H

