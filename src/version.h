/*************************************************************************************************
 *                                                                                                *
 *  file: version.h                                                                               *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2017 Eugene Melnik <eugene@melnik.solutions>                               *
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

namespace Alexandra
{
          // Main information
        const QString appName = "alexandra";
        const QString appNameGui = "Alexandra Video Library";
        const QString appLicense = "GNU GPL v2";
        const QString appWebsite = "http://melnik.solutions/project/alexandra/";
        const QString appDescription = QObject::tr( "Small, fast, but powerful video library, written in C++ with Qt5." );

          // Links
        const QString linkUpdates = "http://melnik.solutions/project/alexandra/updates/";
        const QString linkTranslations = "https://www.transifex.com/eugene-melnik/alexandra-video-library/";

          // Author
        const QString appAuthor = "Eugene Melnik <eugene@melnik.solutions>";
        const QString orgName   = "Eugene Melnik";
        const QString orgDomain = "melnik.solutions";

          // Version
        const quint8  verMajor = 1;
        const quint8  verMinor = 5;
        const quint8  verFix = 1;
        const QString appVersionFull = QString( "%1.%2.%3" ).arg( verMajor ).arg( verMinor ).arg( verFix );
        const QString appBuildDate = QString( __DATE__ );

        const QString databaseHeader = appNameGui + " Database";
        const quint8  databaseVersion = 0x20;

        const quint8 settingsVersion = 0x01;

          // Locales
        struct Locale
        {
            QString title;
            QString selfTitle;
            QString name;
            QString translator;
        };

        const QList<Locale> supportedLocales =
        {
            { "Czech",      "Čeština",     "cs",     "Pavel Fric <pfri@users.sf.net>" },
            { "German",     "Deutsch",     "de",     "Martin Gansser <martinkg@fedoraproject.org> " },
            { "French",     "Français",    "fr",     "David Geiger <david.david@mageialinux-online.org>, "
                                                     "Adrien Daugabel <adrien.d@mageialinux-online.org>" },
            { "English",    "English",     "en",     appAuthor },
            { "Russian",    "Русский",     "ru_RU",  appAuthor },
            { "Ukrainian",  "Українська",  "uk_UA",  appAuthor }
        };

          // Modes
        enum Mode
        {
            ListMode = 0,
            GridMode = 1
        };

          // Themes
        struct Theme
        {
            QString name;
            QString path;
        };

        const QList<Theme> themes =
        {
            { "Alexandra Flat - Coast",             ":/alexandraflat-coast/" },
            { "Alexandra Flat - Rainforest",        ":/alexandraflat-rainforest/" },
            { "QDarkStyleSheet (Colin Duquesnoy)",  ":/qdarkstyle/" }
        };
}

#endif // VERSION_H
