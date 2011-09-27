#include "vee_web_service_adaptor.h"

#include <QtCore/QByteArray>

VeeWebServiceAdaptor::VeeWebServiceAdaptor(VeeWebService * parent) : VeeWebViewAdaptor(parent) {
    connect(parent, SIGNAL(urlChanged(const QUrl &)), SLOT(broadcastUrlChanged(const QUrl &)));
    connect(parent, SIGNAL(linkClicked(const QUrl &)), SLOT(broadcastLinkClicked(const QUrl &)));
}

void VeeWebServiceAdaptor::broadcastUrlChanged(const QUrl & url) {
    QByteArray qAr = url.toEncoded();
    emit urlChanged(QString(qAr));
}

void VeeWebServiceAdaptor::broadcastLinkClicked(const QUrl & url) {
    QByteArray qAr = url.toEncoded();
    emit linkClicked(QString(qAr));
}
