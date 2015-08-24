
#include "commandlineparser.h"
#include "version.h"

#include <QString>
#include <QStringList>
#include <QProcessEnvironment>

CommandLineParser::CommandLineParser() : QCommandLineParser()
{
    setApplicationDescription( Alexandra::appDescription );
    addHelpOption();
    addVersionOption();

    configLocation = new QCommandLineOption( QStringList() << "c" << "config",
                                             QObject::tr( "Specify the location of the configuration file." ),
                                             QString( "/path/to/config" ) );
    addOption( *configLocation );
}

CommandLineParser::~CommandLineParser()
{
    delete configLocation;
}

QString CommandLineParser::GetConfigLocation() const
{
    QString configFile = value( *configLocation );

    if( configFile.isEmpty() )
    {
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
