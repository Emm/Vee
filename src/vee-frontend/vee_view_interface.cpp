#include "vee_view_interface.h"

VeeViewInterface::VeeViewInterface(QProcess* process, const QString &service, const QString &path, const QString & interfaceName, const QDBusConnection &connection, QObject *parent) :
        QDBusAbstractInterface(service, path, interfaceName.toLatin1().constData(), connection, parent),
        mProcess(process) {
    process->setParent(this);
    // FIXME connect process error?
}

VeeViewInterface::~VeeViewInterface() {
}

QDBusPendingReply<> VeeViewInterface::resolve(const QString & url) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(url);
    return asyncCallWithArgumentList(QLatin1String("resolve"), argumentList);
}

QDBusPendingReply<> VeeViewInterface::embed() {
    return asyncCall(QLatin1String("embed"));
}


QString VeeViewInterface::title() const {
    return qvariant_cast< QString >(property("title"));
}
