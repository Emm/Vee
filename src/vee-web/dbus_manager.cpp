#include "dbus_manager.h"
#include "vee_web_view_adaptor_impl.h"

DBusManager::DBusManager(const QString & serviceId, const QString & objectPath) : mServiceId(serviceId), mObjectPath(objectPath) {
}

DBusManager::~DBusManager() {
}

void DBusManager::registerWidget(VeeWebView & view) const {
    new VeeWebViewAdaptorImpl(& view);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(mObjectPath, & view);
    dbus.registerService(mServiceId);
}

void DBusManager::unregisterWidget() const {
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.unregisterObject(mObjectPath);
    dbus.unregisterService(mServiceId);
}
