#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QList>

namespace Alexandra
{
        // Main information
        const QString appName = "alexandra";
        const QString appNameGui = "Alexandra Video Library";
        const QString appAuthor = "Eugene Melnik <jeka7js@gmail.com>";
        const QString appLicense = "GNU GPL v2";
        const QString appWebsite = "http://alexandra-qt.sourceforge.net/";

        // Author
        const QString orgName = "Eugene Melnik";
        const QString orgDomain = "eugene.melnik.com";

        // Version
        const quint8 major = 1;
        const quint8 minor = 2;
        const quint8 fix = 2;
        const QString appVersionFull = QString( "%1.%2.%3" ).arg( major ).arg( minor ).arg( fix );
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

        const QList<Theme> themes = { { "Alexandra Flat - Rainforest", ":/alexandraflat-rainforest/" },
                                      { "QDarkStyleSheet (Colin Duquesnoy)", ":/qdarkstyle/" } };
}

#endif // VERSION_H
