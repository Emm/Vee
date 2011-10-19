#ifndef VEE_WEB_SERVICE_ADAPTOR_H
#define VEE_WEB_SERVICE_ADAPTOR_H

#include "vee_web_view_adaptor.h"

class VeeWebServiceAdaptor : public WebViewAdaptor {

Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.vee.WebView")

public:
    explicit VeeWebServiceAdaptor(WebViewProxy * parent);
    virtual ~VeeWebServiceAdaptor() {};

private slots:
    void broadcastUrlChanged(const QUrl & url);
    void broadcastLinkClicked(const QUrl & url);
};

#endif
