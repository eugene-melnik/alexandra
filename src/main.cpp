/*************************************************************************************************
 *                                                                                                *
 *  file: main.cpp                                                                                *
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

#include "mainwindow/mainwindow.h"
#include "tools/commandlineparser.h"
#include "alexandrasettings.h"
#include "tools/debug.h"
#include "version.h"

#include <QApplication>
#include <QLocale>
#include <QTextCodec>
#include <QTranslator>
#include <QThread>

void LoadLocale( QApplication* app, int localeIndex )
{
    DebugPrintFunc( "::LoadLocale" );

    QString locale;

    if( localeIndex == -1 )
    {
        locale = QLocale::system().name();
    }
    else
    {
        locale = Alexandra::supportedLocales[ localeIndex ].name;
    }

    DebugPrint( "Loading locale: " + locale );

    QTranslator* translator = new QTranslator( app );
    translator->load( QString( ":/lang/alexandra-%1.qm" ).arg( locale ) );
    bool isInstalled = app->installTranslator( translator );

    QTranslator* qt_translator = new QTranslator( app );
    qt_translator->load( QString( ":/lang/qt-%1.qm" ).arg( locale ) );
    app->installTranslator( qt_translator );

    DebugPrint( isInstalled ? "Success" : "Failed!" ); Q_UNUSED(isInstalled)
    DebugPrintFuncDone( "::LoadLocale" );
}

int main( int argc, char** argv )
{
    DebugPrintFunc( "::main" );
    DebugPrint( QString( "IdealThreadCount = %1" ).arg( QThread::idealThreadCount() ) );

      // Configure application
    QApplication::setOrganizationName( Alexandra::orgName );
    QApplication::setOrganizationDomain( Alexandra::orgDomain );
    QApplication::setApplicationName( Alexandra::appNameGui );
    QApplication::setApplicationVersion( Alexandra::appVersionFull );

    QApplication alexandra( argc, argv );

      // Command line options
    CommandLineParser parser;
    parser.process( alexandra );

      // Config file
    AlexandraSettings::Initialize( parser.GetConfigLocation() );

      // Translation
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "utf8" ) );
    LoadLocale( &alexandra, AlexandraSettings::GetInstance()->GetApplicationLocaleIndex() );

      // Run
    MainWindow* mainWindow = new MainWindow();

    if( parser.GetFilmsToAdd().size() > 0 )
    {
        mainWindow->AddFilmsFromOutside( parser.GetFilmsToAdd() );
    }

    mainWindow->show();

    DebugPrintFunc( "alexandra.exec" );
    int res = alexandra.exec();

    DebugPrintL( "Closed" );
    delete mainWindow;
    return( res );
}
