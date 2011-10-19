#include "web_view_proxy_adaptor.h"

#include <QtCore/QByteArray>

WebViewProxyAdaptor::WebViewProxyAdaptor(WebViewProxy * parent) : WebViewAdaptor(parent) {
    connect(parent, SIGNAL(urlChanged(const QUrl &)), SLOT(broadcastUrlChanged(const QUrl &)));
    connect(parent, SIGNAL(linkClicked(const QUrl &)), SLOT(broadcastLinkClicked(const QUrl &)));
}

void WebViewProxyAdaptor::broadcastUrlChanged(const QUrl & url) {
    QByteArray qAr = url.toEncoded();
    emit urlChanged(QString(qAr));
}

void WebViewProxyAdaptor::broadcastLinkClicked(const QUrl & url) {
    QByteArray qAr = url.toEncoded();
    emit linkClicked(QString(qAr));
}
