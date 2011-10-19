#include "vee_web_view_interface.h"
#include <QDBusPendingCall>

VeeWebViewInterface::VeeWebViewInterface(QProcess* process, const QString &service, const QString &path, const QString & interfaceName, const QDBusConnection &connection, QObject *parent) :
    RemoteView(process, service, path, interfaceName, connection, parent) {
}

VeeWebViewInterface::~VeeWebViewInterface() {
}

void VeeWebViewInterface::reload() {
    mRealInterface->asyncCall(QLatin1String("reload"));
}

void VeeWebViewInterface::setHtml(const QString &html) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(html);
    mRealInterface->asyncCallWithArgumentList(QLatin1String("setHtml"), argumentList);
}

void VeeWebViewInterface::stop() {
    mRealInterface->asyncCall(QLatin1String("stop"));
}
