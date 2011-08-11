#ifndef VEE_WEB_VIEW_ADAPTOR_IMPL_H
#define VEE_WEB_VIEW_ADAPTOR_IMPL_H

#include "vee_web_view_adaptor.h"

class VeeWebViewAdaptorImpl : VeeWebViewAdaptor {

Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.vee.web.VeeWebView")

public:
    explicit VeeWebViewAdaptorImpl(VeeWebView * parent);
    virtual ~VeeWebViewAdaptorImpl() {};

private slots:
    void broadcastUrl(const QUrl & url);
};

#endif
