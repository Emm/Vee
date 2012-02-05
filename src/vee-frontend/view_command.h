#ifndef VIEW_COMMAND_H
#define VIEW_COMMAND_H

#include "embed_command.h"

class ViewCommand {

private:

    const EmbedCommand mEmbedCommand;

    const QString mInterfaceName;

    const QString mServiceIdPattern;

    const QString mObjectPath;

public:

    explicit ViewCommand(const EmbedCommand & embedCommand, const QString &
            interfaceName, const QString & serviceIdPattern, const QString &
            objectPath);
    virtual ~ViewCommand();

    const EmbedCommand & embedCommand() const;

    const QString & interfaceName() const;

    const QString & serviceIdPattern() const;

    const QString & objectPath() const;
};
#endif
