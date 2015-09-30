/*************************************************************************************************
 *                                                                                                *
 *  file: version.h                                                                               *
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

#ifndef VERSION_H
#define VERSION_H

#include <QList>
#include <QObject>
#include <QString>

namespace Alexandra
{
        // Main information
        const QString appName = "alexandra";
        const QString appNameGui = "Alexandra Video Library";
        const QString appLicense = "GNU GPL v2";
        const QString appWebsite = "http://alexandra-qt.sourceforge.net/";
        const QString appDescription = QObject::tr( "A small opensource video library with great potential, written in C++ using Qt5 framework." );

        // Author
        const QString appAuthor = "Eugene Melnik <jeka7js@gmail.com>";
        const QString orgName = "Eugene Melnik";
        const QString orgDomain = "eugene.melnik.com";

        // Version
        const quint8 verMajor = 1;
        const quint8 verMinor = 3;
        const quint8 verFix = 0;
        const QString appVersionFull = QString( "%1.%2.%3" ).arg( verMajor ).arg( verMinor ).arg( verFix );
        const QString appBuildDate = QString( __DATE__ );

        const quint8 databaseVersion = 0x10;
        const QString databaseHeader = appNameGui + " Database";

        // Locales
        typedef struct {
            QString title;
            QString selfTitle;
            QString name;
            QString translator;
        } Locale;

        const QList<Locale> supportedLocales = { { "Czech",      "Čeština",     "cs",    "Pavel Fric <pfri@users.sf.net>" },
                                                 { "French",     "Français",    "fr",    "David Geiger <david.david@mageialinux-online.org>, "
                                                                                         "Adrien Daugabel <adrien.d@mageialinux-online.org>" },
                                                 { "English",    "English",     "en",    appAuthor },
                                                 { "Russian",    "Русский",     "ru_RU", appAuthor },
                                                 { "Ukrainian",  "Українська",  "uk_UA", appAuthor } };

        // Modes
        enum Modes {
            ListMode = 0,
            GridMode = 1
        };

        // Themes
        typedef struct {
            QString name;
            QString path;
        } Theme;

        const QList<Theme> themes = { { "Alexandra Flat - Coast", ":/alexandraflat-coast/" },
                                      { "Alexandra Flat - Rainforest", ":/alexandraflat-rainforest/" },
                                      { "QDarkStyleSheet (Colin Duquesnoy)", ":/qdarkstyle/" } };
}

#endif // VERSION_H
