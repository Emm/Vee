#ifndef VEE_VIEW_COMMAND_H
#define VEE_VIEW_COMMAND_H

#include "embed_command.h"

struct ViewCommand {
    EmbedCommand* embedCommand;
    QString interfaceName;
    QString serviceIdPattern;
    QString objectPath;
};
#endif