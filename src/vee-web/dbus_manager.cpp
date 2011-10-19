#include "dbus_manager.h"
#include "vee_web_service_adaptor.h"

DBusManager::DBusManager(const QString & serviceId, const QString & objectPath) : mServiceId(serviceId), mObjectPath(objectPath) {
}

DBusManager::~DBusManager() {
}

void DBusManager::registerWidget(WebViewProxy & view) const {
    new VeeWebServiceAdaptor(& view);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(mObjectPath, & view);
    dbus.registerService(mServiceId);
}

void DBusManager::unregisterWidget() const {
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.unregisterObject(mObjectPath);
    dbus.unregisterService(mServiceId);
}
