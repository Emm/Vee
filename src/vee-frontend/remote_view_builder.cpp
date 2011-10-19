#include "remote_view_builder.h"
#include "web_view.h"
#include "view_types.h"
#include <QDebug>

RemoteViewBuilder::RemoteViewBuilder(const ViewCommand & viewCommand, QObject* parent) :
    ViewBuilder(parent),
    mViewCommand(viewCommand),
    mProcess(NULL) {
    mWatcher.setConnection(QDBusConnection::sessionBus());
}

RemoteViewBuilder::~RemoteViewBuilder() {
    if (mProcess != NULL) {
        // In this case, the builder is waiting for the view to come up, but is
        // being destroyed before it could hand over control of it
        mProcess->terminate();
        delete mProcess;
    }
}

void RemoteViewBuilder::build(const ulong identifier) {
    mProcess = new QProcess(this);
    const QString & executable = mViewCommand.embedCommand->executable();
    QStringList * pArguments = mViewCommand.embedCommand->arguments(identifier);
    const QStringList & arguments = *pArguments; 
    //delete pArguments;
    connect(mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processGotAnError(QProcess::ProcessError)));
    qDebug() << "Identifier " << identifier;
    mService = mViewCommand.serviceIdPattern.arg(identifier);
    qDebug() << "Watching for " << mService;
    mWatcher.addWatchedService(mService);
    connect(& mWatcher, 
            SIGNAL(serviceOwnerChanged(const QString &, const QString &, const QString)),
            this,
            SLOT(serviceIsUp(const QString &, const QString &, const QString)));
    qDebug() << "Starting " << executable << " " << arguments;
    mProcess->start(executable, arguments);
}

void RemoteViewBuilder::processGotAnError(QProcess::ProcessError processError) {
    emit error(processErrorToBuilderError(processError));
    cleanupAfterError();
}

void RemoteViewBuilder::serviceIsUp(const QString & serviceName, const QString & oldOwner, const QString & newOwner) {
    qDebug() << "Service is up";
    if (serviceName != mService) {
        emit error(ViewBuilder::WrongServiceNameError);
        cleanupAfterError();
    }
    else {
        View* view = buildView();
        if (view == NULL) {
            emit error(ViewBuilder::WrongInterfaceNameError);
            cleanupAfterError();
        }
        else {
            emit viewBuilt(view);
            cleanupAfterSuccess();
        }
    }
}

View* RemoteViewBuilder::buildView() {
    View* view;
    if (mViewCommand.interfaceName == WEB_VIEW_TYPE)
        view = new WebView(mProcess, mService, mViewCommand.objectPath,
                mViewCommand.interfaceName, QDBusConnection::sessionBus(), this); 
    else
        view = NULL;
    return view;
}

void RemoteViewBuilder::cleanupAfterSuccess() {
    disconnectAll();
    mWatcher.removeWatchedService(mService);
    mService.clear();
    mProcess = NULL;
}

void RemoteViewBuilder::cleanupAfterError() {
    disconnectAll();
    mWatcher.removeWatchedService(mService);
    mService.clear();
    delete mProcess;
}

void RemoteViewBuilder::disconnectAll() {
    disconnect(& mWatcher, 0, this, 0);
    disconnect(mProcess, 0, this, 0);
}

ViewBuilder::BuilderError RemoteViewBuilder::processErrorToBuilderError(QProcess::ProcessError error) const {
    ViewBuilder::BuilderError newError;
    if (error == QProcess::FailedToStart)
        newError = ViewBuilder::ProcessFailedToStart;
    else if (error == QProcess::Crashed)
        newError = ViewBuilder::ProcessCrashed;
    else
        newError = ViewBuilder::UnknownError;
    return newError;
}

const QString & RemoteViewBuilder::viewType() const {
    qDebug() << "viewType";
    return mViewCommand.interfaceName;
}
