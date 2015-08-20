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
#include "aboutwindow.h"
#include "mediainfo.h"
#include "version.h"

#include <QColor>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QString>

AboutWindow::AboutWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );

    // Base information
    lAppName->setText( Alexandra::appNameGui );
    lAppVersion->setText( tr( "version %1 (build date: %2)" ).arg( Alexandra::appVersionFull, Alexandra::appBuildDate ) );
    lQtVersion->setText( QT_VERSION_STR );
#ifdef __VERSION__
    lGccVersion->setText( __VERSION__ );
#else
    labelGccVersion->hide();
    lGccVersion->hide();
#endif
    lMIVersion->setText( MediaInfo::GetLibraryVersion() );

    lAuthor->setText( Alexandra::appAuthor );
    lLicense->setText( Alexandra::appLicense );

    // Translators
    QString t;

    foreach( Alexandra::Locale loc, Alexandra::supportedLocales )
    {
        t += loc.selfTitle + " (" + loc.name + ") -- " + loc.translator + "\n\n";
    }

    tTranslators->setText( t );
}

void AboutWindow::AboutQt()
{
    QMessageBox::aboutQt( this->parentWidget() );
}

void AboutWindow::showEvent( QShowEvent* event )
{
    // Random image from resources
    image->setPixmap( QPixmap( QString( ":/cats/%1" ).arg( rand() % 6 + 1 ) ) );

    // Random color
//    QColor color( rand()%255, 255, rand()%255 );
//    lAppName->setGraphicsEffect( new EffectDropShadow( 0, 0, color, lAppName ) );
//    lAppVersion->setGraphicsEffect( new EffectDropShadow( 0, 0, color, lAppVersion ) );
//    image->setGraphicsEffect( new EffectDropShadow( 0, 0, color, image ) );

    event->accept();
}
