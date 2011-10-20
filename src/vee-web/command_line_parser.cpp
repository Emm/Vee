#include "command_line_parser.h"
#include "constants.h"

CommandLineParser::CommandLineParser(const QString & appName, const QString & appVersion):
    mParser(new TCLAP::CmdLine(appName.toUtf8().data(), ' ',
            appVersion.toUtf8().data())),
    mWinIdArg(new TCLAP::ValueArg<ulong>("w", "window-id", "X server window ID, for embedding", false, NULL_WINDOW_ID, "integer")),
    mUrlArg(new TCLAP::UnlabeledValueArg<std::string>("url", "URL or filename", false, "", "url")),
    mWindowId(NULL_WINDOW_ID),
    mUrlOrFile(NULL),
    mErrorMessage(NULL)
{
    mParser->add(*mWinIdArg);
    mParser->add(*mUrlArg);
}

CommandLineParser::~CommandLineParser() {
    delete mParser;
    delete mWinIdArg;
    delete mUrlArg;
    delete mUrlOrFile;
    delete mErrorMessage;
}

int CommandLineParser::parse(int argc, char** argv) {
    delete mErrorMessage;
    delete mUrlOrFile;
    try {
        mParser->parse(argc, argv);
        if (mWinIdArg->isSet())
            mWindowId = mWinIdArg->getValue();
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

ulong CommandLineParser::windowId() {
    return mWindowId;
}

QString CommandLineParser::errorMessage() {
    if (mErrorMessage != NULL)
        return QString(*mErrorMessage);
    else
        return QString();
}
