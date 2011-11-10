#include "web_view.h"
#include <QDBusPendingCall>

WebView::WebView(const ViewCommand & viewCommmand, Process* process, QObject* parent) :
    RemoteView(viewCommmand, process, parent) {
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
