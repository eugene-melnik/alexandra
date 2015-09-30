/*************************************************************************************************
 *                                                                                                *
 *  file: aboutwindow.cpp                                                                         *
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

#include "effects/effectdropshadow.h"
#include "network/updatechecker.h"
#include "aboutwindow.h"
#include "mediainfo.h"
#include "version.h"

#include <QColor>
#include <QLabel>
#include <QPixmap>
#include <QString>

AboutWindow::AboutWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );

    // Base information
    lAppName->setText( Alexandra::appNameGui );
    lQtVersion->setText( QT_VERSION_STR );
    lAuthor->setText( Alexandra::appAuthor );
    lLicense->setText( Alexandra::appLicense );

#ifdef PORTABLE_VERSION
    lAppVersion->setText( tr( "version %1 (portable) (build date: %2)" ).arg( Alexandra::appVersionFull, Alexandra::appBuildDate ) );
#else
    lAppVersion->setText( tr( "version %1 (build date: %2)" ).arg( Alexandra::appVersionFull, Alexandra::appBuildDate ) );
#endif // PORTABLE_VERSION

#ifdef __VERSION__
    lGccVersion->setText( __VERSION__ );
#else
    labelGccVersion->hide();
    lGccVersion->hide();
#endif // __VERSION__

#ifdef MEDIAINFO_SUPPORT
    lMIVersion->setText( MediaInfo::GetLibraryVersion() );
#else
    labelMIVersion->hide();
    lMIVersion->hide();
#endif // MEDIAINFO_SUPPORT

    // List of translators
    QString t;

    for( Alexandra::Locale locale : Alexandra::supportedLocales )
    {
        // Print in format "English (en) - Translator Name <tr3000@mail>"
        t += QString( "<b>%1 (%2)</b> &mdash; %3<br/><br/>" ).arg( locale.selfTitle, locale.name,
                                                                   locale.translator.replace( "<", "&lt;" ).replace( ">", "&gt;" ) );
    }

    tTranslators->setText( t );

    // Check for updates
    lLinkLatestVersion->hide();

    UpdateChecker* uc = new UpdateChecker();
    connect( uc, &UpdateChecker::Loaded, this, &AboutWindow::CompareVersions );
    connect( uc, &UpdateChecker::Loaded, uc, &QObject::deleteLater );
    uc->Run();
}

void AboutWindow::show()
{
    // Random picture for each show :)
    image->setPixmap( QPixmap( QString( ":/cats/%1" ).arg( rand() % 6 + 1 ) ) );
    QDialog::show();
}

void AboutWindow::CompareVersions( const QString& latestVersion )
{
    if( latestVersion > Alexandra::appVersionFull )
    {
        lLinkLatestVersion->setText( lLinkLatestVersion->text().arg( latestVersion ) );
        lLinkLatestVersion->show();
    }
}
