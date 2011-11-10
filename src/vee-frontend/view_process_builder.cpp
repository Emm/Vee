#include "view_process_builder.h"
#include "view_process.h"

ViewProcessBuilder::ViewProcessBuilder() : ProcessBuilder() {}

ViewProcessBuilder::~ViewProcessBuilder() {}

Process* ViewProcessBuilder::build() const {
    return new ViewProcess();
}
