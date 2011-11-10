#ifndef PROCESS_BUILDER_H
#define PROCESS_BUILDER_H

#include "process.h"

class ProcessBuilder {

public:

    explicit ProcessBuilder() {};
    virtual ~ProcessBuilder() {};

    virtual Process* build() const = 0;
};

#endif
