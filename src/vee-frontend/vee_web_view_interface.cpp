#include "vee_web_view_interface.h"

VeeWebViewInterface::VeeWebViewInterface(QProcess* process, const QString
        &service, const QString &path, const QString & interfaceName, const
        QDBusConnection &connection, QObject *parent) :
    VeeViewInterface(process, service, path, interfaceName, connection, parent) {
}

VeeWebViewInterface::~VeeWebViewInterface() {
}


QDBusPendingReply<> VeeWebViewInterface::reload() {
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList(QLatin1String("reload"), argumentList);
}

QDBusPendingReply<> VeeWebViewInterface::setHtml(const QString &html) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(html);
    return asyncCallWithArgumentList(QLatin1String("setHtml"), argumentList);
}

QDBusPendingReply<> VeeWebViewInterface::stop() {
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList(QLatin1String("stop"), argumentList);
}
