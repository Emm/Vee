#include <QtTest/QTest>
#include <QDBusConnection>
#include "web_view_proxy_adaptor.h"

#define TEST_SERVICE_ID "org.vee.TestWebViewProxyAdaptor"
#define TEST_SERVICE_PATH "/TestWebViewProxyAdaptor"
#define WEB_VIEW_INTERFACE "org.vee.VeeWebView"

/**
 * DBus interface to WebViewProxy, for testing purposes.
 */
class VeeWebServiceInterface : public QDBusAbstractInterface {

Q_OBJECT

public:
    explicit VeeWebServiceInterface(const QString &service, const QString &path,
            const char * interfaceName, const QDBusConnection &connection,
            QObject *parent) : QDBusAbstractInterface(service, path, interfaceName, connection, parent) {};

    virtual ~VeeWebServiceInterface() {};
signals:
    void urlChanged(const QString & url);
};

/**
 * Tests ViewAdaptor. 
 */
class TestWebViewProxyAdaptor : public QObject {
Q_OBJECT
private:
    WebViewProxy* mProxy;
    VeeWebServiceInterface* mInterface;
    QString* mUrl;

public slots:
    void setUrl(const QString & url) {
        mUrl = new QString(url);
    };

private slots:

    void init() {
        mProxy = new WebViewProxy();
        new WebViewProxyAdaptor(mProxy);
        mUrl = NULL;
        QDBusConnection dbus = QDBusConnection::sessionBus();
        dbus.registerService(TEST_SERVICE_ID);
        dbus.registerObject(TEST_SERVICE_PATH, mProxy);
        QTest::qWait(10);
        mInterface = new VeeWebServiceInterface(TEST_SERVICE_ID, TEST_SERVICE_PATH, WEB_VIEW_INTERFACE, QDBusConnection::sessionBus(), this); 
    };

    void testBroadcastUrl() {
        QDBusConnection dbus = QDBusConnection::sessionBus();
        connect(mInterface, SIGNAL(urlChanged(const QString &)), this, SLOT(setUrl(const QString &)));
        QString url("about:blank");
        mProxy->resolve(url);
        QTest::qWait(10);
        if (mUrl == NULL)
            QFAIL("The broadcastUrl signal wasn't received by the interface");
        else
            QCOMPARE(*mUrl, url);
    };

    void cleanup() {
        QDBusConnection dbus = QDBusConnection::sessionBus();
        dbus.unregisterObject(TEST_SERVICE_PATH);
        dbus.unregisterService(TEST_SERVICE_ID);
        delete mProxy;
        if (mUrl != NULL)
            delete mUrl;
    };
};

QTEST_MAIN(TestWebViewProxyAdaptor)
