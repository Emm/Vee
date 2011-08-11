#include "vee_web_view_adaptor_impl.h"

#include <QtCore/QByteArray>
#include <QDebug>
VeeWebViewAdaptorImpl::VeeWebViewAdaptorImpl(VeeWebView * parent) : VeeWebViewAdaptor(parent) {
    connect(parent, SIGNAL(urlChanged(const QUrl &)), SLOT(broadcastUrl(const QUrl &)));
}

void VeeWebViewAdaptorImpl::broadcastUrl(const QUrl & url) {
    QByteArray qAr = parent()->url().toEncoded();
    emit urlChanged(QString(qAr));
}
