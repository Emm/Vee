#include <QtTest/QTest>
#include <QDBusConnection>
#include "vee_web_view_adaptor_impl.h"

#define TEST_SERVICE_ID "org.vee.web.TestVeeWebViewAdaptorImpl"
#define TEST_SERVICE_PATH "/TestVeeWebViewAdaptorImpl"
#define WEB_VIEW_INTERFACE "org.vee.web.VeeWebView"

/**
 * DBus interface to VeeWebView, for testing purposes.
 */
class VeeWebViewInterface : public QDBusAbstractInterface {

Q_OBJECT

public:
    explicit VeeWebViewInterface(const QString &service, const QString &path, const char * interfaceName, const QDBusConnection &connection, QObject *parent);
    virtual ~VeeWebViewInterface();
signals:
    void urlChanged(const QString & url);
};


VeeWebViewInterface::VeeWebViewInterface(const QString &service, const QString &path, const char * interfaceName, const QDBusConnection &connection, QObject *parent) : QDBusAbstractInterface(service, path, interfaceName, connection, parent) {
}

VeeWebViewInterface::~VeeWebViewInterface() {
}

/**
 * Tests ViewAdaptorImpl. 
 */
class TestVeeWebViewAdaptorImpl : public QObject {
Q_OBJECT
private:
    VeeWebView* mView;
    VeeWebViewInterface* mInterface;
    QString* mUrl;

public slots:
    void setUrl(const QString & url);

private slots:
    void init();
    void testBroadcastUrl();
    void cleanup();
};

void TestVeeWebViewAdaptorImpl::init() {
    mView = new VeeWebView();
    mUrl = NULL;
    new VeeWebViewAdaptorImpl(mView);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerService(TEST_SERVICE_ID);
    dbus.registerObject(TEST_SERVICE_PATH, mView);
    QTest::qWait(10);
    mInterface = new VeeWebViewInterface(TEST_SERVICE_ID, TEST_SERVICE_PATH, WEB_VIEW_INTERFACE, QDBusConnection::sessionBus(), this); 
}

void TestVeeWebViewAdaptorImpl::testBroadcastUrl() {
    QDBusConnection dbus = QDBusConnection::sessionBus();
    connect(mInterface, SIGNAL(urlChanged(const QString &)), this, SLOT(setUrl(const QString &)));
    QString url("about:blank");
    mView->loadUrlOrPath(url);
    QTest::qWait(10);
    if (mUrl == NULL)
        QFAIL("The broadcastUrl signal wasn't received by the interface");
    else
        QCOMPARE(*mUrl, url);
}

void TestVeeWebViewAdaptorImpl::cleanup() {
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.unregisterObject(TEST_SERVICE_PATH);
    dbus.unregisterService(TEST_SERVICE_ID);
    delete mView;
    if (mUrl != NULL)
        delete mUrl;
}

void TestVeeWebViewAdaptorImpl::setUrl(const QString & url) {
    mUrl = new QString(url);
}

QTEST_MAIN(TestVeeWebViewAdaptorImpl)
