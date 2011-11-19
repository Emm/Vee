#include "remote_view.h"
#include <QDBusPendingCall>
#include <QStringList>
#include <QDebug>

RemoteView::RemoteView(const ViewCommand & viewCommand, Process* process, QObject *parent) :
        View(parent),
        mViewCommand(viewCommand),
        mProcess(process),
        mRealInterface(NULL),
        mService(NULL),
        mWatcher(NULL) {
    mProcess->setParent(this);
}


RemoteView::~RemoteView() {
    delete mService;
}

void RemoteView::init(const ulong identifier) {
    const QString & executable = mViewCommand.embedCommand->executable();
    QStringList* pArguments = mViewCommand.embedCommand->arguments(identifier);
    const QStringList & arguments = *pArguments;
    connect(mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processGotAnError(QProcess::ProcessError)));
    mService = new QString(mViewCommand.serviceIdPattern.arg(identifier));

    mWatcher = new QDBusServiceWatcher();
    mWatcher->setConnection(QDBusConnection::sessionBus());
    mWatcher->addWatchedService(*mService);
    connect(mWatcher,
            SIGNAL(serviceOwnerChanged(const QString &, const QString &, const QString)),
            this,
            SLOT(serviceIsUp()));
    qDebug() << "Creating remote view with command " << executable << " " << arguments;
    mProcess->start(executable, arguments);
}

void RemoteView::serviceIsUp() {
    delete mWatcher;
    qDebug() << "Remote view is reachable through DBus";
    mRealInterface = new QDBusInterface(*mService, mViewCommand.objectPath,
                mViewCommand.interfaceName.toLatin1().constData(),
                QDBusConnection::sessionBus(), this);
    connect(mRealInterface, SIGNAL(urlResolved()), this, SIGNAL(urlResolved()));
    connect(mRealInterface, SIGNAL(urlNotResolved()), this, SIGNAL(urlNotResolved()));
    connect(mRealInterface, SIGNAL(urlChanged(QString)), this, SIGNAL(urlChanged(QString)));
    connect(mRealInterface, SIGNAL(titleChanged(QString)), this, SIGNAL(titleChanged(QString)));
    connect(mRealInterface, SIGNAL(error(int, int)), this, SLOT(remoteGotAnError(int, int)));
    emit initialized();
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

void RemoteView::processGotAnError(QProcess::ProcessError processError) {
    emit error(ProcessError, processError);
}
