/*************************************************************************************************
 *                                                                                                *
 *  file: main.cpp                                                                                *
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

#include "alexandrasettings.h"
#include "mainwindow.h"
#include "version.h"

#include <QApplication>
#include <QLocale>
#include <QTextCodec>
#include <QTranslator>
#include <QTime>

#include<QDebug>

void LoadLocale( QApplication* app )
{
    AlexandraSettings s;
    int currentLocaleIndex = s.GetApplicationLocaleIndex();

    QString locale;

    if( currentLocaleIndex == -1 )
    {
        locale = QLocale::system().name();
    }
    else
    {
        locale = Alexandra::supportedLocales[ currentLocaleIndex ].name;
    }

    QTranslator* translator = new QTranslator( app );
    translator->load( QString( ":/lang/alexandra-%1.qm" ).arg( locale ) );
    app->installTranslator( translator );

    QTranslator* qt_translator = new QTranslator( app );
    qt_translator->load( QString( ":/lang/qt-%1.qm" ).arg( locale ) );
    app->installTranslator( qt_translator );
}

int main( int argc, char** argv )
{

QTime a;
a.start();

    // Create seed for the random
    qsrand( QTime::currentTime().msecsSinceStartOfDay() );

    // Configure application
    QApplication::setOrganizationName( Alexandra::orgName );
    QApplication::setOrganizationDomain( Alexandra::orgDomain );
    QApplication::setApplicationName( Alexandra::appNameGui );

    QApplication alexandra( argc, argv );

    // Translation loading
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "utf8" ) );
    LoadLocale( &alexandra );

    // Run
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();

qDebug()<<"Startup time ="<<a.elapsed()<<"msec";// BUG

    return( alexandra.exec() );
}
