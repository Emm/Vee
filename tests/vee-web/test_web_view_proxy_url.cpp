#include <QtTest/QtTest>
#include "web_view_proxy.h"

/**
 * Tests View object with URLs
 */
class TestWebViewProxyUrl: public QObject {
Q_OBJECT

private:
    WebViewProxy* mProxy;
    bool mSuccess;

public slots:
    void succeeded() {
        mSuccess = true;
    };

    void failed() {
        mSuccess = false;
    };

private slots:
    void init() {
        mProxy = new WebViewProxy();
        connect(mProxy, SIGNAL(urlResolved()), this, SLOT(succeeded()));
        connect(mProxy, SIGNAL(urlNotResolved()), this, SLOT(failed()));
    };

    void testCorrectUrl() {
        mSuccess = false;
        mProxy->resolve(QString("about:blank"));
        QTest::qWait(10);
        QCOMPARE(mSuccess, true);
    };
 
    void testWrongUrl() {
        mSuccess = true;
        mProxy->resolve(QString("abut:wrongurl"));
        QTest::qWait(10);
        QCOMPARE(mSuccess, false);
    };

    void cleanup() {
        delete mProxy;
    };
};

QTEST_MAIN(TestWebViewProxyUrl)
