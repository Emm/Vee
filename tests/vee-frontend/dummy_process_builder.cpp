#include "dummy_process_builder.h"
#include "dummy_process.h"

DummyProcessBuilder::DummyProcessBuilder() : ProcessBuilder() {
}

DummyProcessBuilder::~DummyProcessBuilder() {
}

Process* DummyProcessBuilder::build() const {
    return new DummyProcess();
}
