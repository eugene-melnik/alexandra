#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QList>

namespace Alexandra
{
        const QString appName = "alexandra";
        const QString appNameGui = "Alexandra Video Library";
        const QString appAuthor = "Eugene Melnik <jeka7js@gmail.com>";
        const QString appLicense = "GNU GPL v2";
        const QString appWebsite = "http://alexandra-qt.sourceforge.net/";

        const QString orgName = "Eugene Melnik";
        const QString orgDomain = "eugene.melnik.com";

        const quint8 major = 0;
        const quint8 minor = 9;
        const quint8 fix = 8;
        const QString appVersionFull = QString( "%1.%2.%3" ).arg( major ).arg( minor ).arg( fix );
        const QString appBuildDate = QString( __DATE__ );

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
}

#endif // VERSION_H
