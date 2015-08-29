/*************************************************************************************************
 *                                                                                                *
 *  file: commandlineparser.cpp                                                                   *
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

#include "commandlineparser.h"
#include "version.h"

#include <QApplication>
#include <QProcessEnvironment>

CommandLineParser::CommandLineParser() : QCommandLineParser()
{
    setApplicationDescription( Alexandra::appDescription );

    // "-h" and "--help"
    addHelpOption();
    // "-v" and "--version"
    addVersionOption();

    // Adding films from the command line arguments
    addPositionalArgument( "films", "Add films to the database.", "[<file1> [<file2> ...]]" );

    // "-c" and "--config"
    configLocationOption = new QCommandLineOption( QStringList() << "c" << "config",
                                                   "Specify the location of the configuration file.",
                                                   "/path/to/config" );
    addOption( *configLocationOption );
}

CommandLineParser::~CommandLineParser()
{
    delete configLocationOption;
}

QStringList CommandLineParser::GetFilmsToAdd() const
{
    return( positionalArguments() );
}

QString CommandLineParser::GetConfigLocation() const
{
    QString configFile = value( *configLocationOption );

    if( configFile.isEmpty() )
    {
#ifdef PORTABLE_VERSION
        return( qApp->applicationDirPath() + "/data/configuration.conf" );
#endif

#ifdef Q_OS_LINUX
        configFile = QProcessEnvironment::systemEnvironment().value( "XDG_CONFIG_HOME" );

        if( configFile.isEmpty() )
        {
            configFile = QProcessEnvironment::systemEnvironment().value( "HOME" ) + "/.config";
        }
#elif defined( Q_OS_WIN32 )
        configFile = QProcessEnvironment::systemEnvironment().value( "APPDATA" );
#endif
        configFile += "/" + Alexandra::appName + "/configuration.conf";
    }

    return( configFile );
}
