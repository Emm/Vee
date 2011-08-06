#include "view_adaptor_impl.h"

#include <QtCore/QByteArray>
#include <QDebug>
ViewAdaptorImpl::ViewAdaptorImpl(View * parent) : ViewAdaptor(parent) {
    connect(parent, SIGNAL(urlChanged(const QUrl &)), SLOT(broadcastUrl(const QUrl &)));
}

void ViewAdaptorImpl::broadcastUrl(const QUrl & url) {
    QByteArray qAr = parent()->url().toEncoded();
    emit urlChanged(QString(qAr));
}
