#include "vee_view_remote_interface.h"
#include <QDBusPendingCall>

VeeViewRemoteInterface::VeeViewRemoteInterface(QProcess* process, const QString &service, const QString &path, const QString & interfaceName, const QDBusConnection &connection, QObject *parent) :
        VeeViewInterface(parent),
        mProcess(process),
        mRealInterface(new QDBusInterface(service, path, interfaceName.toLatin1().constData(), connection, this)) {
    process->setParent(this);
    connect(mRealInterface, SIGNAL(urlResolved()), this, SIGNAL(urlResolved()));
    connect(mRealInterface, SIGNAL(urlNotResolved()), this, SIGNAL(urlNotResolved()));
    connect(mRealInterface, SIGNAL(urlChanged(QString)), this, SIGNAL(urlChanged(QString)));
    connect(mRealInterface, SIGNAL(titleChanged(QString)), this, SIGNAL(titleChanged(QString)));
    connect(mRealInterface, SIGNAL(error(int, int)), this, SIGNAL(error(int, int)));
}

VeeViewRemoteInterface::~VeeViewRemoteInterface() {
}

void VeeViewRemoteInterface::embed() {
    mRealInterface->asyncCall(QLatin1String("embed"));
}

void VeeViewRemoteInterface::resolve(const QString & url) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(url);
    mRealInterface->asyncCallWithArgumentList(QLatin1String("resolve"), argumentList);
}


QString VeeViewRemoteInterface::interface() const {
    return mRealInterface->interface();
}

QString VeeViewRemoteInterface::title() const {
    return qvariant_cast< QString >(mRealInterface->property("title"));
}

QString VeeViewRemoteInterface::url() const {
    return qvariant_cast< QString >(mRealInterface->property("url"));
}
