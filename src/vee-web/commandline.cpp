#include "commandline.h"

CommandLineParser::CommandLineParser(const QString & appName, const QString & appVersion) {
    mParser = new TCLAP::CmdLine(appName.toUtf8().data(), ' ',
            appVersion.toUtf8().data());
    mWinIdArg = new TCLAP::ValueArg<ulong>("w", "window-id", "X server window ID, for embedding", false, 0, "integer");
    mParser->add(*mWinIdArg);

    mUrlArg = new TCLAP::UnlabeledValueArg<std::string>("url", "URL or filename", true, "", "url");
    mParser->add(*mUrlArg);
    mWindowId = 0;
    mUrlOrFile = NULL;
    mErrorMessage = NULL;
}

CommandLineParser::~CommandLineParser() {
    if (mParser != NULL)
        delete mParser;
    if (mWinIdArg != NULL)
        delete mWinIdArg;
    if (mUrlArg != NULL)
        delete mUrlArg;
    if (mUrlOrFile != NULL)
        delete mUrlOrFile;
    if (mErrorMessage != NULL)
        delete mErrorMessage;
}

int CommandLineParser::parse(int argc, char** argv) {
    try {
        mParser->parse(argc, argv);
        if (mWinIdArg->isSet())
            mWindowId = mWinIdArg->getValue();
        mUrlOrFile = new QString(mUrlArg->getValue().c_str());
        return COMMANDLINE_PARSING_SUCCESSFUL;
    }
    catch(TCLAP::ArgException & e) {
        char* errorString;
        snprintf(errorString, 256, "Error %s for %s\n", e.error().c_str(), e.argId().c_str());
        mErrorMessage = new QString(errorString);
        return COMMANDLINE_PARSING_ERROR;
    }
}

void CommandLineParser::reset() {
    mParser->reset();
    if (mUrlOrFile != NULL)
        delete mUrlOrFile;
    if (mErrorMessage != NULL)
        delete mErrorMessage;
}

const QString & CommandLineParser::urlOrFile() {
    if (mUrlOrFile != NULL)
        return *(new QString(*mUrlOrFile));
    else
        return *(new QString);
}

ulong CommandLineParser::windowId() {
    return mWindowId;
}

const QString & CommandLineParser::errorMessage() {
    if (mErrorMessage != NULL)
        return *(new QString(*mErrorMessage));
    else
        return *(new QString);
}
