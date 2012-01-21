#ifndef DUMMY_PROCESS_H
#define DUMMY_PROCESS_H

#include "process.h"

class DummyProcess : public Process {

Q_OBJECT

private:

    const QString* mExecutable;
    const QStringList* mArguments;

public:

    explicit DummyProcess();

    virtual ~DummyProcess();

    virtual void start(const QString & executable, const QStringList & arguments);

    void emitError(QProcess::ProcessError errorCode);

    void crash();

    void exit();

    const QString* executable();

    const QStringList* arguments();

public slots:

    virtual void terminate();

    virtual QProcess::ProcessState state() const;
};

#endif
