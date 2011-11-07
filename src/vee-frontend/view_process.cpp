#include "view_process.h"

ViewProcess::ViewProcess(QObject* parent) :
    Process(parent),
    mProcess(new QProcess) {
    mProcess->setParent(this);

    connect(mProcess, SIGNAL(error(QProcess::ProcessError)), this, SIGNAL(error(QProcess::ProcessError)));
    connect(mProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SIGNAL(finished(int, QProcess::ExitStatus)));
    connect(mProcess, SIGNAL(started()), this, SIGNAL(started()));
    connect(mProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this, SIGNAL(stateChanged(QProcess::ProcessState)));
}

ViewProcess::~ViewProcess() {}

void ViewProcess::start(const QString & program, const QStringList & arguments) {
    return mProcess->start(program, arguments);
}

void ViewProcess::terminate() {
    mProcess->terminate();
}

QProcess::ProcessState ViewProcess::state() const {
    return mProcess->state();
}
