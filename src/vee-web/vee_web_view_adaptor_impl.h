#ifndef VEE_WEB_VIEW_ADAPTOR_IMPL_H
#define VEE_WEB_VIEW_ADAPTOR_IMPL_H

#include "vee_web_view_adaptor.h"

class VeeWebViewAdaptorImpl : VeeWebViewAdaptor {

Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.vee.VeeWebView")

public:
    explicit VeeWebViewAdaptorImpl(VeeWebView * parent);
    virtual ~VeeWebViewAdaptorImpl() {};

private slots:
    void broadcastUrlChanged(const QUrl & url);
    void broadcastLinkClicked(const QUrl & url);
};

#endif
