/*************************************************************************************************
 *                                                                                                *
 *  file: alexandra.cpp                                                                           *
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

#include "mainwindow.h"
#include "version.h"

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QTime>

int main( int argc, char** argv )
{
    // Create seed for the random
    qsrand( QTime::currentTime().msecsSinceStartOfDay() );

    // Configure application
    QApplication::setOrganizationName( Alexandra::orgName );
    QApplication::setOrganizationDomain( Alexandra::orgDomain );
    QApplication::setApplicationName( Alexandra::appNameGui );

    QApplication alexandra( argc, argv );

    // Translation loading
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "utf8" ) );

    QTranslator* translator = new QTranslator();
    translator->load( ":/lang/alexandra-locale.qm" );
    alexandra.installTranslator( translator );
    QTranslator* qt_translator = new QTranslator();
    qt_translator->load( ":/lang/qt-locale.qm" );
    alexandra.installTranslator( qt_translator );

    // Run
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
    return( alexandra.exec() );
}
