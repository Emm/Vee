#ifndef DUMMY_PROCESS_BUILDER_H
#define DUMMY_PROCESS_BUILDER_H

#include "process_builder.h"

class DummyProcessBuilder : public ProcessBuilder {

public:

    explicit DummyProcessBuilder();
    virtual ~DummyProcessBuilder();

    virtual Process* build() const;
};

#endif
