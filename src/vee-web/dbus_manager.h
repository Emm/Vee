#ifndef DBUS_MANAGER_H
#define DBUS_MANAGER_H

#include <cstdlib>
#include "vee_web_view.h"

class DBusManager {
private:
    QString mServiceId;
    QString mObjectPath;
public:
    explicit DBusManager(const QString & serviceIdTemplate, const QString & objectPath, const ulong instanceId);
    virtual ~DBusManager();
    void registerWidget(VeeWebView* widget);
    void unregisterWidget();
};

#endif
