#include "view_builder.h"
#include "vee_web_view_interface.h"
#include <QDebug>

ViewBuilder::ViewBuilder(const VeeViewCommand & veeViewCommand, QObject* parent) :
    QObject(parent),
    mVeeViewCommand(veeViewCommand),
    mProcess(NULL) {
    mWatcher.setConnection(QDBusConnection::sessionBus());
}

ViewBuilder::~ViewBuilder() {
    if (mProcess != NULL) {
        // In this case, the builder is waiting for the view to come up, but is
        // being destroyed before it could hand over control of it
        mProcess->terminate();
        delete mProcess;
    }
}

void ViewBuilder::build(const ulong identifier) {
    mProcess = new QProcess(this);
    const QString & executable = mVeeViewCommand.embedCommand->executable();
    QStringList * pArguments = mVeeViewCommand.embedCommand->arguments(identifier);
    const QStringList & arguments = *pArguments; 
    //delete pArguments;
    connect(mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processGotAnError(QProcess::ProcessError)));
    qDebug() << "Identifier " << identifier;
    mService = mVeeViewCommand.serviceIdPattern.arg(identifier);
    qDebug() << "Watching for " << mService;
    mWatcher.addWatchedService(mService);
    connect(& mWatcher, 
            SIGNAL(serviceOwnerChanged(const QString &, const QString &, const QString)),
            this,
            SLOT(serviceIsUp(const QString &, const QString &, const QString)));
    qDebug() << "Starting " << executable << " " << arguments;
    mProcess->start(executable, arguments);
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
    if (mVeeViewCommand.interfaceName == "org.vee.VeeWebView")
        view = new VeeWebViewInterface(mProcess, mService, mVeeViewCommand.objectPath,
                mVeeViewCommand.interfaceName, QDBusConnection::sessionBus(), this); 
    else if (mVeeViewCommand.interfaceName == "org.vee.VeeView")
        view = new VeeViewInterface(mProcess, mService, mVeeViewCommand.objectPath,
                mVeeViewCommand.interfaceName, QDBusConnection::sessionBus(), this); 
    else
        view = NULL;
    return view;
}

void ViewBuilder::cleanupAfterSuccess() {
    disconnectAll();
    mWatcher.removeWatchedService(mService);
    mService.clear();
    mProcess = NULL;
}

void ViewBuilder::cleanupAfterError() {
    disconnectAll();
    mWatcher.removeWatchedService(mService);
    mService.clear();
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

const QString & ViewBuilder::viewType() const {
    qDebug() << "viewType";
    return mVeeViewCommand.interfaceName;
}
