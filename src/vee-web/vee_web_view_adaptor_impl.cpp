#include "vee_web_view_adaptor_impl.h"

#include <QtCore/QByteArray>

VeeWebViewAdaptorImpl::VeeWebViewAdaptorImpl(VeeWebView * parent) : VeeWebViewAdaptor(parent) {
    connect(parent, SIGNAL(urlChanged(const QUrl &)), SLOT(broadcastUrlChanged(const QUrl &)));
    connect(parent, SIGNAL(linkClicked(const QUrl &)), SLOT(broadcastLinkClicked(const QUrl &)));
}

void VeeWebViewAdaptorImpl::broadcastUrlChanged(const QUrl & url) {
    QByteArray qAr = url.toEncoded();
    emit urlChanged(QString(qAr));
}

void VeeWebViewAdaptorImpl::broadcastLinkClicked(const QUrl & url) {
    QByteArray qAr = url.toEncoded();
    emit linkClicked(QString(qAr));
}
