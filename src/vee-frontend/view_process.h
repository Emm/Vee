#ifndef VIEW_PROCESS_H
#define VIEW_PROCESS_H

#include "process.h"

class ViewProcess : public Process {

Q_OBJECT

private:

    QProcess* mProcess;

public:

    explicit ViewProcess(QObject* parent=0);

    virtual ~ViewProcess();

    virtual void start(const QString & program, const QStringList & arguments);

public slots:

    virtual void terminate();

    virtual QProcess::ProcessState state () const;
};

#endif
