#ifndef VIEW_BULDER_H
#define VIEW_BULDER_H

#include <QDBusServiceWatcher>
#include <QProcess>
#include "embedcommand.h"
#include "vee_view_interface.h"
#include "vee_view_command.h"

class ViewBuilder : public QObject {
    Q_OBJECT

public:
    enum BuilderError {
        WrongServiceNameError = 0,
        WrongInterfaceNameError = 1,
        ProcessFailedToStart = 2,
        ProcessCrashed = 3,
        UnknownError = 4
    };
    explicit ViewBuilder(const VeeViewCommand & veeViewCommand, QObject* parent=0);
    virtual ~ViewBuilder();
    void build(const ulong identifier);
    const QString & viewType() const;

private:
    const VeeViewCommand & mVeeViewCommand;
    QString mService;
    QDBusServiceWatcher mWatcher;
    QProcess* mProcess;

    VeeViewInterface* buildView();
    void cleanupAfterSuccess();
    void cleanupAfterError();
    void disconnectAll();
    BuilderError processErrorToBuilderError(QProcess::ProcessError error) const;

private slots:
    void processGotAnError(QProcess::ProcessError processError);
    void serviceIsUp(const QString & serviceName, const QString & oldOwner, const QString & newOwner);

signals:
    void error(ViewBuilder::BuilderError error);
    void viewBuilt(VeeViewInterface* view);
};

#endif
