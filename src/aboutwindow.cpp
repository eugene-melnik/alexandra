/*************************************************************************************************
 *                                                                                                *
 *  file: aboutwindow.cpp                                                                         *
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

#include "aboutwindow.h"
#include "version.h"

#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QString>

AboutWindow::AboutWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );

    lAppName->setText( Alexandra::appNameGui );
    lAppVersion->setText( tr( "version %1 (build date: %2)" ).arg( Alexandra::appVersionFull, Alexandra::appBuildDate ) );

    lQtVersion->setText( tr( "<b>Qt version:</b> %1" ).arg( QT_VERSION_STR ) );
    lAuthor->setText( tr( "<b>Author(s):</b> %1" ).arg( Alexandra::appAuthor ) );
    lLicense->setText( tr( "<b>License:</b> %1" ).arg( Alexandra::appLicense ) );
    lAddress->setText( tr( "<b>Address:</b> <a href=\"%1\">Github</a>" ).arg( Alexandra::appAddress ) );
}

void AboutWindow::AboutQt()
{
    QMessageBox::aboutQt( this->parentWidget() );
}

void AboutWindow::showEvent( QShowEvent* event )
{
    // Random image from resources
    image->setPixmap( QPixmap( QString( ":/cats/%1" ).arg( rand() % 6 + 1 ) ) );
    event->accept();
}
