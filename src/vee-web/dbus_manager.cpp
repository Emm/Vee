#include "dbus_manager.h"
#include "vee_web_view_adaptor_impl.h"

DBusManager::DBusManager(const QString & serviceIdTemplate, const QString & objectPath, const ulong instanceId) {
    mServiceId = serviceIdTemplate.arg(instanceId);
    mObjectPath = objectPath;
}

DBusManager::~DBusManager() {
}

void DBusManager::registerWidget(VeeWebView* view) {
    new VeeWebViewAdaptorImpl(view);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(mObjectPath, view);
    dbus.registerService(mServiceId);
}

void DBusManager::unregisterWidget() {
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.unregisterObject(mObjectPath);
    dbus.unregisterService(mServiceId);
}
