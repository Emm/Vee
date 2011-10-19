#include "web_view.h"
#include <QDBusPendingCall>

WebView::WebView(QProcess* process, const QString &service, const QString &path, const QString & interfaceName, const QDBusConnection &connection, QObject *parent) :
    RemoteView(process, service, path, interfaceName, connection, parent) {
}

WebView::~WebView() {
}

void WebView::reload() {
    mRealInterface->asyncCall(QLatin1String("reload"));
}

void WebView::setHtml(const QString &html) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(html);
    mRealInterface->asyncCallWithArgumentList(QLatin1String("setHtml"), argumentList);
}

void WebView::stop() {
    mRealInterface->asyncCall(QLatin1String("stop"));
}
