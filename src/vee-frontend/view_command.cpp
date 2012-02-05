#include "view_command.h"

ViewCommand::ViewCommand(const EmbedCommand & embedCommand, const QString &
        interfaceName, const QString & serviceIdPattern, const QString &
        objectPath) :
    mEmbedCommand(embedCommand),
    mInterfaceName(interfaceName),
    mServiceIdPattern(serviceIdPattern),
    mObjectPath(objectPath) {
}

ViewCommand::~ViewCommand() {
}

const EmbedCommand & ViewCommand::embedCommand() const {
    return mEmbedCommand;
}

const QString & ViewCommand::interfaceName() const {
    return mInterfaceName;
}

const QString & ViewCommand::serviceIdPattern() const {
    return mServiceIdPattern;
}

const QString & ViewCommand::objectPath() const {
    return mObjectPath;
}
