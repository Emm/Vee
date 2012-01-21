#ifndef REMOTE_VIEW_H
#define REMOTE_VIEW_H

#include <QDBusInterface>
#include <QDBusServiceWatcher>

#include "process_builder.h"
#include "view_command.h"
#include "view.h"

class RemoteView : public View {
    Q_OBJECT

private:

    void destroyWatcher();

protected:
    const ViewCommand & mViewCommand;
    Process* mProcess;
    QDBusInterface* mRealInterface;
    QString* mService;
    QDBusServiceWatcher* mWatcher;

public:
    explicit RemoteView(const ViewCommand & viewCommand, Process* process,
            QObject* parent = 0);
    virtual ~RemoteView();

    virtual void init(const ulong identifier);

    Q_PROPERTY(QString title READ title)
    QString title() const;

    Q_PROPERTY(QString url READ url)
    QString url() const;

    Q_PROPERTY(QString interface READ interface)
    QString interface() const;

public slots:
    void embed();
    virtual void resolve(const QString &url);
    void serviceIsUp();
    virtual void remoteGotAnError(int errorType, int errorCode);
    virtual void updateIcon();
    virtual void remoteLoadFinished(bool ok);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif
