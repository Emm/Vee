#ifndef WEB_VIEW_PROXY_BUILDER_H
#define WEB_VIEW_PROXY_BUILDER_H

#include <cstdlib>

#include "web_view_proxy.h"
#include "dbus_manager.h"

class WebViewProxyBuilder {
private:
    const QString& mUrlOrFile;
    const ulong mWindowId;
    const DBusManager* mDBusManager;

    WebViewProxy* buildView();
    inline bool shouldEmbed() const;
    const QString & readHtmlFromStdin() const;

public:
    explicit WebViewProxyBuilder(const QString & urlOrFile, const ulong windowId = 0, const DBusManager* dbusManager = NULL);
    virtual ~WebViewProxyBuilder();
    WebViewProxy* build();
};

#endif
