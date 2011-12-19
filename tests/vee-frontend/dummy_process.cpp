#include "dummy_process.h"

DummyProcess::DummyProcess() : Process() {
}

DummyProcess::~DummyProcess() {
}

void DummyProcess::start(const QString & executable, const QStringList & arguments) {
    mExecutable = &executable;
    mArguments = &arguments;
}

const QString* DummyProcess::executable() {
    return mExecutable;
}

const QStringList* DummyProcess::arguments() {
    return mArguments;
}

void DummyProcess::terminate() {
}

QProcess::ProcessState DummyProcess::state() const {
    return QProcess::NotRunning;
}
