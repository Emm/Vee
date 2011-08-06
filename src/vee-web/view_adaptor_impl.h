#ifndef VIEW_ADAPTOR_IMPL_H
#define VIEW_ADAPTOR_IMPL_H

#include "view_adaptor.h"

class ViewAdaptorImpl : ViewAdaptor {

Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.vee.web.View")

public:
    explicit ViewAdaptorImpl(View * parent);
    virtual ~ViewAdaptorImpl() {};

private slots:
    void broadcastUrl(const QUrl & url);
};

#endif
