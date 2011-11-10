#ifndef VIEW_PROCESS_BUILDER_H
#define VIEW_PROCESS_BUILDER_H

#include "process_builder.h"

class ViewProcessBuilder : public ProcessBuilder {

public:

    explicit ViewProcessBuilder();

    virtual ~ViewProcessBuilder();

    virtual Process* build() const;
};

#endif
