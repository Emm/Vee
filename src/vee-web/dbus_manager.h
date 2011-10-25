#ifndef DBUS_MANAGER_H
#define DBUS_MANAGER_H

#include <cstdlib>
#include "web_view_proxy.h"

/*! The DBusManager registers and unregisters a WebViewProxy with DBus.
 *
 */

class DBusManager {
private:
    const QString mServiceId;
    const QString mObjectPath;
public:
    explicit DBusManager(const QString & serviceId, const QString & objectPath);
    virtual ~DBusManager();
    void registerWidget(WebViewProxy & widget) const;
    void unregisterWidget() const;
};

#endif
