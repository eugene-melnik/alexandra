/*************************************************************************************************
 *                                                                                                *
 *  file: settingswindow.h                                                                        *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>                                     *
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

#include "ui_settingswindow.h"

#include "alexandrasettings.h"

#include <QDialog>
#include <QShowEvent>
#include <QString>

class SettingsWindow : public QDialog, public Ui::SettingsWindow
{
    Q_OBJECT

    public:
        SettingsWindow( AlexandraSettings* s, QWidget* parent = nullptr );

    protected:
        void showEvent( QShowEvent* event );

    signals:
        void DatabaseSettingsChanged();
        void SettingsChanged();
        void EraseDatabase();

    private slots:
        void OkButtonClicked();
        void SetIsSettingsChanged();
        void SetIsNeedReboot();
        void SetIsDatabaseSettingsChanged();

        void SetDefaultExternalPlayer();

        void OpenDatabaseFile();
        void SelectColorUnavailable();
        void CreateDatabase();
        void EraseDatabaseQuestion();
        void OpenPostersFolder();

    private:
        void ConfigureApplicationTab();
        void ReconfigureApplicationTab();
        void ConfigureDatabaseTab();
        void ReconfigureDatabaseTab();

        // Variables
        AlexandraSettings* settings;
        bool isSettingsChanged;
        bool isDatabaseSettingsChanged;
        bool isNeedReboot;

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

        const QList<QString> appStyles = { "CDE",
                                           "Cleanlooks",
                                           "GTK+",
                                           "Motif",
                                           "Plastique",
                                           "Windows" };
};

#endif // SETTINGSWINDOW_H
