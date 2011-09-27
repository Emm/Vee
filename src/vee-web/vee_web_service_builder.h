#ifndef WIDGET_FACTORY_H
#define WIDGET_FACTORY_H

#include <cstdlib>

#include "vee_web_service.h"
#include "dbus_manager.h"

class VeeWebServiceBuilder {
private:
    const QString& mUrlOrFile;
    const ulong mWindowId;
    const DBusManager* mDBusManager;

    VeeWebService* buildView();
    inline bool shouldEmbed() const;
    const QString & readHtmlFromStdin() const;

public:
    explicit VeeWebServiceBuilder(const QString & urlOrFile, const ulong windowId = 0, const DBusManager* dbusManager = NULL);
    virtual ~VeeWebServiceBuilder();
    VeeWebService* build();
};

#endif
