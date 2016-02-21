/*************************************************************************************************
 *                                                                                                *
 *  file: splashscreen.cpp                                                                        *
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

#include "splashscreen.h"
#include "effects/effectdropshadow.h"
#include "version.h"

#include <QDesktopWidget>
#include <QTimer>


SplashScreen::SplashScreen( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );

      // On top + no decoration
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );

      // Center of the screen
    QRect rect = frameGeometry();
    rect.moveCenter( QDesktopWidget().availableGeometry().center() );
    move( rect.topLeft() );

      // Information
    lAppName->setGraphicsEffect( new EffectDropShadow( 0, 0, "white", lAppName ) );
    lAppName->setText( Alexandra::appNameGui );

    lVersion->setGraphicsEffect( new EffectDropShadow( 0, 0, "white", lVersion ) );
    lVersion->setText( QString( "version %1").arg( Alexandra::appVersionFull ) );

    lAuthor->setGraphicsEffect( new EffectDropShadow( 0, 0, "white", lAuthor ) );
    lAuthor->setText( Alexandra::appAuthor );

    lWebsite->setGraphicsEffect( new EffectDropShadow( 0, 0, "white", lWebsite ) );
    lWebsite->setText( Alexandra::appWebsite );

    lLoading->setGraphicsEffect( new EffectDropShadow( 0, 0, "white", lLoading ) );
}


void SplashScreen::show()
{
    QDialog::show();
    QTimer::singleShot( 1000, this, SLOT( Close() ) ); // 1 second delay
}


void SplashScreen::Close()
{
    close();
    deleteLater();
}

