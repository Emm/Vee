#include <QTest>
#include <QDBusServiceWatcher>
#include <QDBusConnection>
#include "dbus_manager.h"

class TestDbusManager : public QObject {
Q_OBJECT

    DBusManager* mManager;
    QString* mServiceId;
    QString* mObjectPath;
    bool mRegistered;
    bool mUnregistered;

public slots:
    void serviceRegistered() {
        mRegistered = true;
    }

    void serviceUnregistered() {
        mUnregistered = true;
    }

private slots:
    void init() {
        mServiceId = new QString("org.vee.vee-web-test");
        mObjectPath = new QString("/test");
        mManager = new DBusManager(*mServiceId, *mObjectPath);
        mRegistered = false;
        mUnregistered = false;
    }

    void testRegistration() {
        WebViewProxy proxy;
        QDBusServiceWatcher watcher(*mServiceId, QDBusConnection::sessionBus());

        connect(&watcher, SIGNAL(serviceRegistered(const QString &)), this, SLOT(serviceRegistered()));
        watcher.setWatchMode(QDBusServiceWatcher::WatchForRegistration);
        mManager->registerWidget(proxy);
        QTest::qWait(5);
        QVERIFY(mRegistered);

        connect(&watcher, SIGNAL(serviceUnregistered(const QString &)), this, SLOT(serviceUnregistered()));
        watcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
        mManager->unregisterWidget();
        QTest::qWait(5);
        QVERIFY(mUnregistered);
    }

    void cleanup() {
        delete mManager;
        delete mServiceId;
        delete mObjectPath;
        mRegistered = false;
        mUnregistered = false;
    }
};

QTEST_MAIN(TestDbusManager)
