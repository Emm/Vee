#include "dummy_process.h"

DummyProcess::DummyProcess() : Process() {
}

DummyProcess::~DummyProcess() {
}

void DummyProcess::start(const QString & executable, const QStringList & arguments) {
    mExecutable = executable;
    mArguments = arguments;
}

const QString DummyProcess::executable() const {
    return mExecutable;
}

const QStringList DummyProcess::arguments() const {
    return mArguments;
}

void DummyProcess::terminate() {
}

QProcess::ProcessState DummyProcess::state() const {
    return QProcess::NotRunning;
}

void DummyProcess::emitError(QProcess::ProcessError errorCode) {
    emit error(errorCode);
}

void DummyProcess::crash() {
    emit finished(-1, QProcess::CrashExit);
}

void DummyProcess::exit() {
    emit finished(0, QProcess::NormalExit);
}
