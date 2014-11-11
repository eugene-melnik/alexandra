#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QList>

namespace Alexandra
{
        const QString appName = QString( "alexandra" );
        const QString appNameGui = QString( "Alexandra Video Library" );
        const QString appAuthor = QString( "Eugene Melnik (jeka7js@gmail.com)" );
        const QString appLicense = QString( "GNU GPL v2" );
        const QString appAddress = QString( "https://github.com/jeka-js/alexandra" );

        const QString orgName = "The Higggs Development";
        const QString orgDomain = "higggs.com";

        const quint8 major = 0;
        const quint8 minor = 9;
        const quint8 fix = 0;
        const QString appVersionFull = QString( "%1.%2.%3" ).arg( major ).arg( minor ).arg( fix );

        typedef struct {
            QString title;
            QString name;
            QString translator;
        } Locale;

        const QList<Locale> supportedLocales = { { "English",    "en_US", appAuthor },
                                                 { "Russian",    "ru_RU", appAuthor },
                                                 { "Ukrainian",  "uk_UA", appAuthor } };
}

#endif // VERSION_H
