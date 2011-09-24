#ifndef WIDGET_FACTORY_H
#define WIDGET_FACTORY_H

#include <cstdlib>

#include "vee_web_view.h"
#include "dbus_manager.h"

class WidgetBuilder {
private:
    const QString& mUrlOrFile;
    const ulong mWindowId;
    const DBusManager* mDBusManager;

    VeeWebView* buildView();
    inline bool shouldEmbed() const;
    const QString & readHtmlFromStdin() const;

public:
    explicit WidgetBuilder(const QString & urlOrFile, const ulong windowId = 0, const DBusManager* dbusManager = NULL);
    virtual ~WidgetBuilder();
    QWidget* build();
};

#endif
