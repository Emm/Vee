#ifndef VEE_VIEW_COMMAND_H
#define VEE_VIEW_COMMAND_H

#include "embedcommand.h"

struct VeeViewCommand {
    EmbedCommand* embedCommand;
    QString interfaceName;
    QString serviceIdPattern;
    QString objectPath;
};
#endif
