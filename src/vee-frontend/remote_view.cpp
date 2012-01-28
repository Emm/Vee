#include "remote_view.h"
#include <QDBusPendingCall>
#include <QStringList>
#include <QFileInfo>
#include <QDebug>

RemoteView::RemoteView(const ViewCommand & viewCommand, Process* process, QObject *parent) :
        View(parent),
        mViewCommand(viewCommand),
        mProcess(process),
        mRealInterface(NULL),
        mWatcher(NULL) {
    mProcess->setParent(this);
}


RemoteView::~RemoteView() {
    destroyWatcher();
}

void RemoteView::destroyWatcher() {
    if (mWatcher != NULL) {
        mWatcher->removeWatchedService(mService);
        mWatcher->disconnect();
        delete mWatcher;
        qDebug() << "Destroyed watcher";
        mWatcher = NULL;
    }
}

bool RemoteView::ensureCanLaunchExecutable(const QString & executable) {
    QFileInfo fileInfo(executable);

    bool canLaunchExecutable;
    if (!fileInfo.isFile()) {
        emit error(View::CommandError, 1);
        canLaunchExecutable = false;
    }
    else if (!fileInfo.isExecutable()) {
        emit error(View::CommandError, 2);
        canLaunchExecutable = false;
    }
    else {
        canLaunchExecutable = true;
    }
    return canLaunchExecutable;
}

void RemoteView::init(const ulong identifier) {
    const QString & executable = mViewCommand.embedCommand->executable();

    if (!ensureCanLaunchExecutable(executable)) {
        return;
    }
    QStringList arguments = mViewCommand.embedCommand->arguments(identifier);
    // Don't connect to the process error signal, we need to wait until the
    // process emits its finished() signal before handling a crash
    connect(mProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    mService = QString(mViewCommand.serviceIdPattern.arg(identifier));

    mWatcher = new QDBusServiceWatcher(mService, QDBusConnection::sessionBus(), QDBusServiceWatcher::WatchForRegistration, this);
    connect(mWatcher,
            SIGNAL(serviceRegistered(const QString &)),
            this,
            SLOT(serviceIsUp()));
    qDebug() << "Creating remote view with command " << executable << " " << arguments;
    mProcess->start(executable, arguments);
}

void RemoteView::serviceIsUp() {
    destroyWatcher();
    qDebug() << "Remote view is reachable through DBus";
    mRealInterface = new QDBusInterface(mService, mViewCommand.objectPath,
                mViewCommand.interfaceName.toLatin1().constData(),
                QDBusConnection::sessionBus(), this);
    connect(mRealInterface, SIGNAL(urlResolved()), this, SIGNAL(urlResolved()));
    connect(mRealInterface, SIGNAL(urlNotResolved()), this, SIGNAL(urlNotResolved()));
    connect(mRealInterface, SIGNAL(urlChanged(QString)), this, SIGNAL(urlChanged(QString)));
    connect(mRealInterface, SIGNAL(titleChanged(QString)), this, SIGNAL(titleChanged(QString)));
    connect(mRealInterface, SIGNAL(iconChanged()), this, SLOT(updateIcon()));
    connect(mRealInterface, SIGNAL(error(int, int)), this, SLOT(remoteGotAnError(int, int)));
    connect(mRealInterface, SIGNAL(loadStarted()), this, SIGNAL(loadStarted()));
    connect(mRealInterface, SIGNAL(loadProgress(int)), this, SIGNAL(loadProgress(int)));
    connect(mRealInterface, SIGNAL(loadFinished(bool)), this, SLOT(remoteLoadFinished(bool)));
    emit initialized();
}

void RemoteView::updateIcon() {
    emit(iconChanged());
}

void RemoteView::remoteLoadFinished(bool ok) {
    emit loadFinished(ok);
}

void RemoteView::embed() {
    mRealInterface->asyncCall(QLatin1String("embed"));
}

void RemoteView::resolve(const QString & url) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(url);
    qDebug() << "Resolving " << url;
    mRealInterface->asyncCallWithArgumentList(QLatin1String("resolve"), argumentList);
}

QString RemoteView::interface() const {
    return mRealInterface->interface();
}

QString RemoteView::title() const {
    return qvariant_cast< QString >(mRealInterface->property("title"));
}

QString RemoteView::url() const {
    return qvariant_cast< QString >(mRealInterface->property("url"));
}

void RemoteView::remoteGotAnError(int errorType, int errorCode) {
    emit error(UnknownError, errorCode);
}

void RemoteView::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    // Systematically emit crash as we shouldn't be getting the signal except
    // when something very wrong happend
    emit error(ProcessError, QProcess::Crashed);
}
