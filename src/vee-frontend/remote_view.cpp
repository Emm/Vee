#include "remote_view.h"
#include <QDBusPendingCall>

RemoteView::RemoteView(QProcess* process, const QString &service, const QString &path, const QString & interfaceName, const QDBusConnection &connection, QObject *parent) :
        View(parent),
        mProcess(process),
        mRealInterface(new QDBusInterface(service, path, interfaceName.toLatin1().constData(), connection, this)) {
    mProcess->setParent(this);
    connect(mRealInterface, SIGNAL(urlResolved()), this, SIGNAL(urlResolved()));
    connect(mRealInterface, SIGNAL(urlNotResolved()), this, SIGNAL(urlNotResolved()));
    connect(mRealInterface, SIGNAL(urlChanged(QString)), this, SIGNAL(urlChanged(QString)));
    connect(mRealInterface, SIGNAL(titleChanged(QString)), this, SIGNAL(titleChanged(QString)));
    connect(mRealInterface, SIGNAL(error(int, int)), this, SIGNAL(error(int, int)));
}

RemoteView::~RemoteView() {
}

void RemoteView::embed() {
    mRealInterface->asyncCall(QLatin1String("embed"));
}

void RemoteView::resolve(const QString & url) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(url);
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
