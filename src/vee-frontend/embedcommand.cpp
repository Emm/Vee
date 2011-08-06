#include "embedcommand.h"

EmbedCommand::EmbedCommand(QString& executable) {
    mExecutable = &executable;
    mArguments = new QStringList();
    mWinIdArgPos = -1;
}

void EmbedCommand::addArgument(QString& argument) {
    *mArguments << argument;
}

void EmbedCommand::addWinId() {
    mWinIdArgPos = mArguments->size();
}

QString& EmbedCommand::executable() {
    return *mExecutable;
}

QStringList* EmbedCommand::arguments(ulong winId) {
    QStringList* arguments = new QStringList();
    arguments->reserve(mArguments->size() + 1);
    for (int i = 0 ; i < mArguments->size() ; i++) {
        if (i == mWinIdArgPos)
            *arguments << QString::number(winId); 
        *arguments << mArguments->at(i); 
    }
    return arguments;
}

void EmbedCommand::operator<< (QString argument) {
    addArgument(argument);
}

void EmbedCommand::operator<< (int WIN_ID) {
    addWinId();
}
