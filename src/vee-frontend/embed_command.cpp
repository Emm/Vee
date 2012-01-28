#include "embed_command.h"

EmbedCommand::EmbedCommand(const QString& executable) :
    mExecutable(executable),
    mWinIdArgPos(-1) {
}

EmbedCommand::~EmbedCommand() {
}

void EmbedCommand::addArgument(const QString& argument) {
    mArguments << argument;
}

void EmbedCommand::addWinId() {
    mWinIdArgPos = mArguments.size();
    mArguments << NULL;
}

const QString & EmbedCommand::executable() const {
    return mExecutable;
}

QStringList EmbedCommand::arguments(ulong winId) const {
    QStringList arguments;
    arguments.reserve(mArguments.size() + 1);
    for (int i = 0 ; i < mArguments.size() ; i++) {
        if (i == mWinIdArgPos)
            arguments << QString::number(winId);
        else
            arguments << mArguments.at(i);
    }
    return arguments;
}

void EmbedCommand::operator<< (QString argument) {
    addArgument(argument);
}

void EmbedCommand::operator<< (int WIN_ID) {
    addWinId();
}
