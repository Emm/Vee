#ifndef VEE_WEB_SERVICE_ADAPTOR_H
#define VEE_WEB_SERVICE_ADAPTOR_H

#include "web_view_adaptor.h"

class WebViewProxyAdaptor : public WebViewAdaptor {

Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.vee.WebView")

public:
    explicit WebViewProxyAdaptor(WebViewProxy * parent);
    virtual ~WebViewProxyAdaptor() {};

private slots:
    void broadcastUrlChanged(const QUrl & url);
    void broadcastLinkClicked(const QUrl & url);
};

#endif
