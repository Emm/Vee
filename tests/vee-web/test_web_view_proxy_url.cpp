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
    void setSuccess(bool success) {
        mSuccess = success;
    };

private slots:
    void init() {
        mProxy = new WebViewProxy();
        connect(mProxy, SIGNAL(loadFinished(bool)), this, SLOT(setSuccess(bool)));
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
