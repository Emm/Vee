#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include <QString>
#include <tclap/CmdLine.h>

#include <cstdlib>

#define COMMANDLINE_PARSING_SUCCESS 0
#define COMMANDLINE_PARSING_ERROR 1

/**
 * Parses the command-line arguments.
 * Returns 0 if the parsing succeeded
 */
class CommandLineParser {
private:
    TCLAP::CmdLine* mParser;
    TCLAP::UnlabeledValueArg<std::string>* mUrlArg;
    ulong mWindowId;
    QString* mUrlOrFile;
    QString* mErrorMessage;

public:
    explicit CommandLineParser(const QString & appName, const QString & appVersion);
    virtual ~CommandLineParser();

    int parse(int argc, char** argv);
    void reset();

    QString urlOrFile();
    QString errorMessage();
};

#endif
