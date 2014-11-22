/*************************************************************************************************
 *                                                                                                *
 *  file: settingswindow.h                                                                        *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014 Eugene Melnik <jeka7js@gmail.com>                                          *
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

#include <QDialog>
#include <QSettings>
#include <QShowEvent>
#include <QString>

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
        void OpenPostersFolder();

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
