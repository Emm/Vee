#ifndef REMOTE_VIEW_BULDER_H
#define REMOTE_VIEW_BULDER_H

#include <QDBusServiceWatcher>
#include "process.h"
#include "view_builder.h"
#include "embed_command.h"
#include "view.h"
#include "view_command.h"

class RemoteViewBuilder : public ViewBuilder {
    Q_OBJECT

public:
    explicit RemoteViewBuilder(const ViewCommand & viewCommand, QObject* parent=0);
    virtual ~RemoteViewBuilder();
    virtual void build(const ulong identifier);
    const virtual QString & viewType() const;

private:
    const ViewCommand & mViewCommand;
    QString mService;
    QDBusServiceWatcher mWatcher;
    Process* mProcess;

    View* buildView();
    void cleanupAfterSuccess();
    void cleanupAfterError();
    void disconnectAll();
    ViewBuilder::BuilderError processErrorToBuilderError(QProcess::ProcessError error) const;

private slots:
    void processGotAnError(QProcess::ProcessError processError);
    void serviceIsUp(const QString & serviceName, const QString & oldOwner, const QString & newOwner);
};

#endif
