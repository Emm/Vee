#ifndef PROCESS_H
#define PROCESS_H

// Abstract Process class, to facilitate unit testing

#include <QProcess>

class Process : public QObject {

Q_OBJECT

public:

    explicit Process(QObject* parent=0) : QObject(parent) {}

    virtual ~Process() {}

    virtual void start(const QString & program, const QStringList & arguments) = 0;

public slots:

    virtual void terminate() = 0;

    virtual QProcess::ProcessState state() const = 0;

signals:

    void error(QProcess::ProcessError error);

    void finished(int exitCode, QProcess::ExitStatus exitStatus);

    void started();

    void stateChanged(QProcess::ProcessState newState);

};

#endif
