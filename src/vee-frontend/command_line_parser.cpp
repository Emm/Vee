#include "command_line_parser.h"

CommandLineParser::CommandLineParser(const QString & appName, const QString & appVersion):
    mParser(new TCLAP::CmdLine(appName.toUtf8().data(), ' ',
            appVersion.toUtf8().data())),
    mUrlArg(new TCLAP::UnlabeledValueArg<std::string>("url", "URL or filename", true, "", "url")),
    mUrlOrFile(NULL),
    mErrorMessage(NULL)
{
    mParser->add(*mUrlArg);
}

CommandLineParser::~CommandLineParser() {
    delete mParser;
    delete mUrlArg;
    delete mUrlOrFile;
    delete mErrorMessage;
}

int CommandLineParser::parse(int argc, char** argv) {
    delete mErrorMessage;
    delete mUrlOrFile;
    try {
        mParser->parse(argc, argv);
        if (mUrlArg->isSet()) {
            mUrlOrFile = new QString(mUrlArg->getValue().c_str());
        }
        return COMMANDLINE_PARSING_SUCCESS;
    }
    catch(TCLAP::ArgException & e) {
        char* errorString = NULL;
        snprintf(errorString, 256, "Error %s for %s\n", e.error().c_str(), e.argId().c_str());
        mErrorMessage = new QString(errorString);
        return COMMANDLINE_PARSING_ERROR;
    }
}

void CommandLineParser::reset() {
    mParser->reset();
}

QString CommandLineParser::urlOrFile() {
    if (mUrlOrFile != NULL)
        return QString(*mUrlOrFile);
    else
        return QString();
}

QString CommandLineParser::errorMessage() {
    if (mErrorMessage != NULL)
        return QString(*mErrorMessage);
    else
        return QString();
}
