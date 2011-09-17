#include "view_builder.h"
#include "vee_web_view_interface.h"
#include <QDebug>

ViewBuilder::ViewBuilder(EmbedCommand* command, const QString &
        interfaceName, const QString & serviceIdPattern, const QString &
        objectPath, QObject* parent) :
    QObject(parent),
    mCommand(command),
    mInterfaceName(interfaceName),
    mServiceIdPattern(serviceIdPattern),
    mObjectPath(objectPath),
    mProcess(NULL) {
}

ViewBuilder::~ViewBuilder() {
    delete mCommand;
    if (mProcess != NULL) {
        // In this case, the builder is waiting for the view to come up, but is
        // being destroyed before it could hand over control of it
        mProcess->terminate();
        delete mProcess;
    }
}

void ViewBuilder::build(const ulong identifier) {
    mProcess = new QProcess(this);
    const QString & executable = mCommand->executable();
    QStringList * pArguments = mCommand->arguments(identifier);
    const QStringList & arguments = *pArguments; 
    mProcess->start(executable, arguments);
    //delete pArguments;
    connect(mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processGotAnError(QProcess::ProcessError)));
    mService = mServiceIdPattern.arg(identifier);
    qDebug() << "Watching for " << mService;
    mWatcher.addWatchedService(mService);
    connect(& mWatcher, 
            SIGNAL(serviceOwnerChanged(const QString &, const QString &, const QString)),
            this,
            SLOT(serviceIsUp(const QString &, const QString &, const QString)));
}

void ViewBuilder::processGotAnError(QProcess::ProcessError processError) {
    emit error(processErrorToBuilderError(processError));
    cleanupAfterError();
}

void ViewBuilder::serviceIsUp(const QString & serviceName, const QString & oldOwner, const QString & newOwner) {
    qDebug() << "Service is up";
    if (serviceName != mService) {
        emit error(ViewBuilder::WrongServiceNameError);
        cleanupAfterError();
    }
    else {
        VeeViewInterface* view = buildView();
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

VeeViewInterface* ViewBuilder::buildView() {
    VeeViewInterface* view;
    if (mInterfaceName == "org.vee.VeeWebView")
        view = new VeeWebViewInterface(mProcess, mService, mObjectPath,
                mInterfaceName, QDBusConnection::sessionBus(), this); 
    else if (mInterfaceName == "org.vee.VeeView")
        view = new VeeViewInterface(mProcess, mService, mObjectPath,
                mInterfaceName, QDBusConnection::sessionBus(), this); 
    else
        view = NULL;
    return view;
}

void ViewBuilder::cleanupAfterSuccess() {
    disconnectAll();
    mWatcher.removeWatchedService(mService);
    mService = QString();
    mProcess = NULL;
}

void ViewBuilder::cleanupAfterError() {
    disconnectAll();
    mWatcher.removeWatchedService(mService);
    mService = QString();
    delete mProcess;
}

void ViewBuilder::disconnectAll() {
    disconnect(& mWatcher, 0, this, 0);
    disconnect(mProcess, 0, this, 0);
}

ViewBuilder::BuilderError ViewBuilder::processErrorToBuilderError(QProcess::ProcessError error) const {
    ViewBuilder::BuilderError newError;
    if (error == QProcess::FailedToStart)
        newError = ViewBuilder::ProcessFailedToStart;
    else if (error == QProcess::Crashed)
        newError = ViewBuilder::ProcessCrashed;
    else
        newError = ViewBuilder::UnknownError;
    return newError;
}
