#ifndef DBUS_MANAGER_H
#define DBUS_MANAGER_H

#include <cstdlib>
#include "vee_web_view.h"

class DBusManager {
private:
    const QString & mServiceId;
    const QString & mObjectPath;
public:
    explicit DBusManager(const QString & serviceId, const QString & objectPath);
    virtual ~DBusManager();
    void registerWidget(VeeWebView & widget) const;
    void unregisterWidget() const;
};

#endif
