
#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QCommandLineParser>

class CommandLineParser : public QCommandLineParser
{
    public:
        CommandLineParser();
        ~CommandLineParser();

        QString GetConfigLocation() const;

    private:
        QCommandLineOption* configLocation = nullptr;
};

#endif // COMMANDLINEPARSER_H
